#include "SDL.h"
#include <windows.h>
#include<gl/glew.h>

#include<iostream>
#include"ShaderLoader.h"
#include"Vector.h"
#include"Matrix.h"
#include"Matrix4f.h"

// Headers for OpenNI
#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnHash.h>
#include <XnLog.h>

// Header for NITE
#include "XnVNite.h"
// local header
#include "PointHandler.h"

#define CHECK_RC(rc, what)											\
	if (rc != XN_STATUS_OK)											\
	{																\
		printf("%s failed: %s\n", what, xnGetStatusString(rc));		\
		return rc;													\
	}

#define CHECK_ERRORS(rc, errors, what)		\
	if (rc == XN_STATUS_NO_NODE_PRESENT)	\
	{										\
		XnChar strError[1024];				\
		errors.ToString(strError, 1024);	\
		printf("%s\n", strError);			\
		return (rc);						\
	}



typedef enum
{
	IN_SESSION,
	NOT_IN_SESSION,
	QUICK_REFOCUS
} SessionState;

bool Running = true;
SDL_Surface* Surf_Display = NULL;

// OpenNI objects
xn::Context g_Context;
xn::ScriptNode g_ScriptNode;
xn::DepthGenerator g_DepthGenerator;
xn::GestureGenerator g_GestureGenerator;

// NITE objects
XnVSessionManager* g_pSessionManager;
XnVFlowRouter* g_pFlowRouter;

// the drawer
PointHandler* pointHandler;

// xml to initialize OpenNI
#define SAMPLE_XML_PATH "Sample-Tracking.xml"

SessionState g_SessionState = NOT_IN_SESSION;

GLuint vPos;
GLuint vao; //vertex array object
GLfloat xRes;
GLfloat yRes;
GLuint matLoc;
GLuint programObject;

GLuint buffer;

Vector position = Vector();

float vertices[] = { 0.0, 0.0, 0.0, 1.0,
					10.0, 10.0, 0.0, 1.0,
					10.0, 0.0, 0.0, 1.0};


void PrintSessionState(SessionState eState)
{
	switch (eState)
	{
		case IN_SESSION:
			std::cout<<"Tracking hands";
			break;
		case NOT_IN_SESSION:
			std::cout<<"Perform click or wave gestures to track hand";
			break;
		case QUICK_REFOCUS:
			std::cout<<"Raise your hand for it to be identified, or perform click or wave gestures";
			break;
	}

}

void setupVAO(){

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER,	sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(vPos);
	glVertexAttribPointer(
		vPos,				// attribute 0. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		0,					// normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
}

void CleanupExit()
{
	g_ScriptNode.Release();
	g_DepthGenerator.Release();
	g_GestureGenerator.Release();
	g_Context.Release();

	exit (1);
}

// Callback for when the focus is in progress
void XN_CALLBACK_TYPE FocusProgress(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt)
{
//	printf("Focus progress: %s @(%f,%f,%f): %f\n", strFocus, ptPosition.X, ptPosition.Y, ptPosition.Z, fProgress);
}
// callback for session start
void XN_CALLBACK_TYPE SessionStarting(const XnPoint3D& ptPosition, void* UserCxt)
{
	printf("Session start: (%f,%f,%f)\n", ptPosition.X, ptPosition.Y, ptPosition.Z);
	g_SessionState = IN_SESSION;
}
// Callback for session end
void XN_CALLBACK_TYPE SessionEnding(void* UserCxt)
{
	printf("Session end\n");
	g_SessionState = NOT_IN_SESSION;
}
void XN_CALLBACK_TYPE NoHands(void* UserCxt)
{
	if (g_SessionState != NOT_IN_SESSION)
	{
		printf("Quick refocus\n");
		g_SessionState = QUICK_REFOCUS;
	}
}

void XN_CALLBACK_TYPE GestureIntermediateStageCompletedHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
	printf("Gesture %s: Intermediate stage complete (%f,%f,%f)\n", strGesture, pPosition->X, pPosition->Y, pPosition->Z);
}
void XN_CALLBACK_TYPE GestureReadyForNextIntermediateStageHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
	printf("Gesture %s: Ready for next intermediate stage (%f,%f,%f)\n", strGesture, pPosition->X, pPosition->Y, pPosition->Z);
}
void XN_CALLBACK_TYPE GestureProgressHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, XnFloat fProgress, void* pCookie)
{
	printf("Gesture %s progress: %f (%f,%f,%f)\n", strGesture, fProgress, pPosition->X, pPosition->Y, pPosition->Z);
}

int configKinect(){
		
	XnStatus rc = XN_STATUS_OK;
	xn::EnumerationErrors errors;

	// Initialize OpenNI
	rc = g_Context.InitFromXmlFile(SAMPLE_XML_PATH, g_ScriptNode, &errors);
	CHECK_ERRORS(rc, errors, "InitFromXmlFile");
	CHECK_RC(rc, "InitFromXmlFile");

	rc = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
	CHECK_RC(rc, "Find depth generator");
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_GESTURE, g_GestureGenerator);
	CHECK_RC(rc, "Find gesture generator");

	XnCallbackHandle hGestureIntermediateStageCompleted, hGestureProgress, hGestureReadyForNextIntermediateStage;
	g_GestureGenerator.RegisterToGestureIntermediateStageCompleted(GestureIntermediateStageCompletedHandler, NULL, hGestureIntermediateStageCompleted);
	g_GestureGenerator.RegisterToGestureReadyForNextIntermediateStage(GestureReadyForNextIntermediateStageHandler, NULL, hGestureReadyForNextIntermediateStage);
	g_GestureGenerator.RegisterGestureCallbacks(NULL, GestureProgressHandler, NULL, hGestureProgress);

	// Create NITE objects
	g_pSessionManager = new XnVSessionManager;
	rc = g_pSessionManager->Initialize(&g_Context, "Click,Wave", "RaiseHand");
	CHECK_RC(rc, "SessionManager::Initialize");

	g_pSessionManager->RegisterSession(NULL, SessionStarting, SessionEnding, FocusProgress);

	pointHandler = new PointHandler(20, g_DepthGenerator); 
	g_pFlowRouter = new XnVFlowRouter;
	g_pFlowRouter->SetActive(pointHandler);

	g_pSessionManager->AddListener(g_pFlowRouter);

	pointHandler->RegisterNoPoints(NULL, NoHands);

	// Initialization done. Start generating
	rc = g_Context.StartGeneratingAll();
	CHECK_RC(rc, "StartGenerating");

	return rc;
}

void draw() {
	glClear( GL_COLOR_BUFFER_BIT );

	XnMapOutputMode mode;
	g_DepthGenerator.GetMapOutputMode(mode);

	Matrix4f projectionMatrix = Matrix4f();
	projectionMatrix.Ortho(0, mode.nXRes, mode.nYRes, 0, -1.0, 1.0);

	Vector primaryPoint;
	primaryPoint = pointHandler->getPrimaryPoint();
	Vector movement = primaryPoint - position;
	movement.normalize();
	position += movement;
	position.print();

	Matrix4f modelViewMatrix;
	modelViewMatrix.translate(position.getX(), position.getY(), 0);

	Matrix4f modelViewProjectionMatrix;
	modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
	//glUniformMatrix4fv(matLoc, 1, GL_FALSE, modelViewProjectionMatrix); 
	// Use our shader
	glUseProgram(programObject);

	modelViewProjectionMatrix.uniformMatrix(matLoc);

	// Draw the triangle
	glDrawArrays(GL_LINE_LOOP, 0, 3);

	g_Context.WaitOneUpdateAll(g_DepthGenerator);
	// Update NITE tree
	g_pSessionManager->Update(&g_Context);
	PrintSessionState(g_SessionState);

	SDL_GL_SwapBuffers();
}

void cleanup() {
    SDL_FreeSurface(Surf_Display);
    SDL_Quit();
}

void processEvent(SDL_Event* Event) {
    if(Event->type == SDL_QUIT) {
        Running = false;
    }
}


void testMatrices( Matrix4f test, Matrix4f otherTest){
	Matrix4f result;


	std::cout<<"test:\n";
	test.print();
 	std::cout<<"other test:\n";
	otherTest.print();
	result = test * otherTest;
	std::cout<<"test * other test =\n";
	result.print();

	result.identity();
	std::cout<<"identity\n";
	result.print();

	result = test + otherTest;
	std::cout<<"test + otherTest\n";
	result.print();

	result.Ortho(0, 10, 0, 10, -1, 1);
	std::cout<<"ortho(0,10,0,10,-1,1)\n";
	result.print();

	Vector r = result * Vector(10,0,0);
	std::cout<<"orthor * (10,0,0)\n";
	r.print();

	r = result * Vector(0,10,0);
	std::cout<<"orthor * (0, 10,0)\n";
	r.print();

	r = result * Vector(0,0,10);
	std::cout<<"orthor * (1,0,0)\n";
	r.print();

	result.translate(10, 20, 30);
	std::cout<<"translate(10,20,30)\n";
	result.print();
}

bool init() {

/*	Matrix4f t;
	t.identity();
	Matrix4f t2;
	t2.Ortho(0, 100, 0, 100, -1, 1);
	
	testMatrices(t, t2);*/
	if( configKinect() != XN_STATUS_OK ){
		std::cout<<"failed to configure kinect";
		return false;
	}

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    if((Surf_Display = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL) {
        return false;
    }

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	  exit(1);
	}

	glClearColor(0, 0, 0, 0);

    glViewport(0, 0, 640, 480);

	// Create and compile the GLSL program from the shaders
	programObject = ShaderLoader::createShader( "vertexTest.txt", "fragmentTest.txt" );
	vPos = glGetAttribLocation(programObject, "position");
	matLoc = glGetUniformLocation(programObject, "modelViewProjectionMatrix");

	setupVAO();


    return true;
}

void update(){
}

int execute() {
    if(init() == false) {
        return -1;
    }

    SDL_Event Event;

    while(Running) {
        while(SDL_PollEvent(&Event)) {
            processEvent(&Event);
        }

        update();
        draw();
    }

    cleanup();

    return 0;
}


int main(int argc, char ** argv)
{

	execute();
	return 0;
}
