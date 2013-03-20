#ifndef __KINECT_HANDLER__
#define __KINECT_HANDLER__

#include<iostream>

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

// OpenNI objects
extern xn::Context g_Context;
extern xn::ScriptNode g_ScriptNode;
extern xn::DepthGenerator g_DepthGenerator;
extern xn::GestureGenerator g_GestureGenerator;

// NITE objects
extern XnVSessionManager* g_pSessionManager;
extern XnVFlowRouter* g_pFlowRouter;

// the drawer
extern PointHandler* pointHandler;

// xml to initialize OpenNI
#define SAMPLE_XML_PATH "Sample-Tracking.xml"

extern SessionState g_SessionState;

int configKinect();

void CleanupExit();
void PrintSessionState(SessionState eState);

#endif