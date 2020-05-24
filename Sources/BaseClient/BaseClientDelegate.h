#ifndef CBASECLIENTDELEGATE_H
#define CBASECLIENTDELEGATE_H
#include "Common\MyTypes.h"
#include "BaseClient_Defs.h"
//#include "Clock\Clock.h"

class CClock;

class CBaseClientDelegate
{
public:
	virtual void CBaseClient_Event_ConnectionState(ClientConnectionState_t ConnectionState, CClock * pElapsedTime) = NULL;
	virtual void CBaseClient_Event_FrameReceived(TU08_t * pBuffer, TU32_t Size) = NULL;
	virtual void CBaseClient_Event_FrameSendResult(bool SendSuccess, TU08_t * pBuffer, TU32_t Size, TU32_t FrameIDentifier) = NULL;
};
#endif