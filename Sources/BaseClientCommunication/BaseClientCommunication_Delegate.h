#ifndef CBASECLIENTCOMMUNICATION_DELEGATE_H
#define CBASECLIENTCOMMUNICATION_DELEGATE_H
#include "Common\MyTypes.h"

class CBaseClientCommunication_Delegate
{
public:
	virtual void Event_FrameSendResult(TU08_t * pBuffer, TU32_t BufferSize, TU32_t NBBytesSent, TU32_t FrameIDentifier) = NULL;
	virtual void Event_FrameReceived(TU08_t * pBuffer, TU32_t Size, TU32_t SizeWaited) = NULL;
};
#endif