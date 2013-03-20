#include "SDL.h"
#include <windows.h>
#include<gl/glew.h>

#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<vector>

#include"ShaderLoader.h"
#include"Vector.h"
#include"Matrix.h"
#include"Matrix4f.h"
#include"Particle.h"
#include "KinectHandler.h"

bool Running = true;
SDL_Surface* Surf_Display = NULL;


bool useKinect = false;

GLuint vPos;
GLfloat xRes;
GLfloat yRes;
GLuint matLoc;
GLuint programObject;

Vector position = Vector();

std::vector<Particle*> particles;

int frames = 0;

void draw() {
	glClear( GL_COLOR_BUFFER_BIT );

	Matrix4f projectionMatrix = Matrix4f();

	if( useKinect){
		XnMapOutputMode mode;
		g_DepthGenerator.GetMapOutputMode(mode);
		projectionMatrix.Ortho(0, mode.nXRes, mode.nYRes, 0, -1.0, 1.0);
	}else{
		projectionMatrix.Ortho(0, 640, 480, 0, -1.0, 1.0);
	}

	Vector primaryPoint;
	if( useKinect){
		primaryPoint = pointHandler->getPrimaryPoint();
	}else{
		int x,y;
		SDL_GetMouseState( &x, &y);
		primaryPoint.setX((GLfloat)x);
		primaryPoint.setY((GLfloat)y);
	}

	// Use our shader
	glUseProgram(programObject);

	for( std::vector<Particle*>::iterator it = particles.begin(); it != particles.end(); it++){
		Vector movement = primaryPoint - (*it)->getPosition();
		movement.normalize();
		(*it)->setVelocity(movement);
		(*it)->update();
		//(*it)->getPosition().print();
		(*it)->draw(vPos, matLoc, projectionMatrix);
	}

	if( useKinect){
		g_Context.WaitOneUpdateAll(g_DepthGenerator);
		// Update NITE tree
		g_pSessionManager->Update(&g_Context);
		PrintSessionState(g_SessionState);
	}

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

bool init() {

	srand( time( NULL));
	if( configKinect() != XN_STATUS_OK ){
		std::cout<<"failed to configure kinect";
		useKinect = false;
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

	for( int i = 0; i < 1000; i++){
		particles.push_back(new Particle( *(new Vector(rand() % 640, rand() % 480, 0, 1))));
	}

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
