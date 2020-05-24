#ifndef CBASECONNECTEDCLIENT_DELEGATE_H
#define CBASECONNECTEDCLIENT_DELEGATE_H
#include "Common\MyTypes.h"

#include "BaseClientCommunication\BaseClientCommunication_Delegate.h"
#include "BaseConnectedClient\BaseConnectedClient_Defs.h"

class CBaseConnectedClient_Delegate : public CBaseClientCommunication_Delegate
{
public:
	virtual void Event_ConnectionStateChanged(BaseConnectedClientStatus_t NewConnectionState) = NULL;
};
#endif