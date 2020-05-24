#ifndef CBASECOMMUNICATION_DELEGATE_H
#define CBASECOMMUNICATION_DELEGATE_H
#include "Common\MyTypes.h"

class CBaseCommunicationDelegate
{
	virtual void BaseCommunication_Event_FrameReceived(TU08_t * pBuffer, TU32_t Size) = NULL;
	virtual void BaseCommunication_Event_FrameSent(bool SendSuccess, TU08_t * pBuffer, TU32_t Size, TU32_t FrameIDentifier) = NULL;
};
#endif