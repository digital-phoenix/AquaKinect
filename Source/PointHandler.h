#ifndef _POINT_HANDLER_
#define _POINT_HANDLER_

#include <map>
#include <list>
#include <XnCppWrapper.h>
#include <XnVPointControl.h>


#define DEBUG_PRINT true
/**
 * This is a point control, which stores the history of every point
 * It can draw all the points as well as the depth map.
 */
class PointHandler : public XnVPointControl
{
public:
	PointHandler(XnUInt32 nHistorySize, xn::DepthGenerator depthGenerator);
	virtual ~PointHandler();

	/**
	 * Handle a new message.
	 */
	void Update(XnVMessage* pMessage);

	void OnPrimaryPointUpdate(const XnVHandPointContext *pContext); 

	void OnPrimaryPointReplace(XnUInt32 nOldId, const XnVHandPointContext *pContext); 

	void OnPrimaryPointDestroy(XnUInt32 nID); 

	XnPoint3D getPrimaryPoint();

	void  OnPrimaryPointCreate(const XnVHandPointContext *pContext, const XnPoint3D &ptSessionStarter); 

protected:

	// Source of the depth map
	xn::DepthGenerator m_DepthGenerator;
	XnFloat* m_pfPositionBuffer;

	XnPoint3D primaryPoint;
};

#endif
