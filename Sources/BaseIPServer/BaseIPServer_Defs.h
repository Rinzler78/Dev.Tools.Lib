#ifndef CBASEIPSERVER_DEFS_H
#define CBASEIPSERVER_DEFS_H
#include "Common\MyTypes.h"

typedef enum CBaseIPServer_Events
{
	CBaseIPServer_Events_Exit,
	CBaseIPServer_Events_WaitForClientConnection,
	MAX_CBaseIPServer_Events
}CBaseIPServer_Events_t;

static const TC08_t CBaseIPServer_Events_STR[MAX_CBaseIPServer_Events][30]
={"Exit", "Wait for Client Connection"};

typedef enum CBaseIPServer_ServerState
{
	CBaseIPServer_ServerState_Stoped,
	CBaseIPServer_ServerState_Started,
	CBaseIPServer_ServerState_Listen,
	CBaseIPServer_ServerState_Accept,
	MAX_CBaseIPServer_ServerState
}CBaseIPServer_ServerState_t;

static const TC08_t CBaseIPServer_ServerState_STR [MAX_CBaseIPServer_ServerState][15]
={"Stoped", "Started", "Listen", "Accept"};
#endif