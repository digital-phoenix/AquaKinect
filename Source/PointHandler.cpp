#include "PointHandler.h"
#include "XnVDepthMessage.h"
#include <XnVHandPointContext.h>

// Constructor. Receives the number of previous positions to store per hand,
// and a source for depth map
PointHandler::PointHandler(XnUInt32 nHistory, xn::DepthGenerator depthGenerator) :
	XnVPointControl("PointHandler"),m_DepthGenerator(depthGenerator)
{
	primaryPoint.X = 0;
	primaryPoint.Y = 0;
	primaryPoint.Z = 0;
}

// Destructor. Clear all data structures
PointHandler::~PointHandler()
{
}


// Handle a new Message
void PointHandler::Update(XnVMessage* pMessage)
{
	// PointControl's Update calls all callbacks for each hand
	XnVPointControl::Update(pMessage);

}

void PointHandler::OnPrimaryPointUpdate(const XnVHandPointContext *pContext){
	// positions are kept in projective coordinates, since they are only used for drawing
	XnPoint3D ptProjective(pContext->ptPosition);
	
	printf("** %d\n", pContext->nID);
	if (DEBUG_PRINT)printf("Point (%f,%f,%f)", ptProjective.X, ptProjective.Y, ptProjective.Z);
	m_DepthGenerator.ConvertRealWorldToProjective(1, &ptProjective, &ptProjective);
	if (DEBUG_PRINT)printf(" -> (%f,%f,%f)\n", ptProjective.X, ptProjective.Y, ptProjective.Z);

	primaryPoint = ptProjective;
	primaryPoint.Z = 0;
}

void PointHandler::OnPrimaryPointCreate(const XnVHandPointContext *pContext, const XnPoint3D &ptSessionStarter){
	OnPrimaryPointUpdate(pContext);
}

void PointHandler::OnPrimaryPointReplace(XnUInt32 nOldId, const XnVHandPointContext *pContext){
	OnPrimaryPointUpdate(pContext);
}

void PointHandler::OnPrimaryPointDestroy(XnUInt32 nID){
}

XnPoint3D PointHandler::getPrimaryPoint(){
	return primaryPoint;
}
