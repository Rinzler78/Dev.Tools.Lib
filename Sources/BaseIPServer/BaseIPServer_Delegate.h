#ifndef CBASEIPSERVER_DELEGATE_H
#define CBASEIPSERVER_DELEGATE_H
#include "Common\MyTypes.h"
#include "BaseIPServer_Defs.h"

class CBaseIPServerDelegate
{
public:
	virtual CBaseIPServerDelegate_Event_ServerState(CBaseIPServer_ServerState_t ConnectionState) = NULL;
	virtual CBaseIPServerDelegate_Event_ConnectionAccepted(SOCKET NewClientSocket) = NULL;
}

#endif