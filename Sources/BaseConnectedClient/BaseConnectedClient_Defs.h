#ifndef CBASECONNECTEDCLIENT_DEFS_H
#define CBASECONNECTEDCLIENT_DEFS_H
#include "Common\MyTypes.h"
#include "BaseClientCommunication\BaseClientCommunication_Defs.h"

/** ConnectionType */
typedef enum ConnectionType
{
	ConnectionType_Serial,
	ConnectionType_IP,
	MAX_ConnectionType
}ConnectionType_t;
static const char ConnectionType_STR_TAB[MAX_ConnectionType][10] = {"Serial", "IP"};

/** Connection Status */
typedef enum BaseConnectedClientStatus
{
	BaseConnectedClientStatus_TryConnect,
	BaseConnectedClientStatus_Connected,
	BaseConnectedClientStatus_ConnectionFailure,
	BaseConnectedClientStatus_TryDisconnect,
	BaseConnectedClientStatus_Disconnected,
	BaseConnectedClientStatus_DisconnectionFailure,
	MAX_BaseConnectedClientStatus

}BaseConnectedClientStatus_t;

static const TC08_t BaseConnectedClientStatus_STR_TAB [MAX_BaseConnectedClientStatus][30]
= {"Try Connect", "Connected", "Connection Failure", "Try DisConnect", "Disconnected", "Disconnection Failure"};

/** Private Events */
typedef enum BaseConnectedClient_Event
{
	BaseConnectedClient_Event_Init = MAX_BaseClientCommunication_Event,
	BaseConnectedClient_Event_Connection,
	BaseConnectedClient_Event_Disconnection,
	BaseConnectedClient_Event_Reconnection,
	MAX_BaseConnectedClient_Event

}BaseConnectedClient_Event_t;

static const char BaseConnectedClient_Event_STR_TAB[MAX_BaseConnectedClient_Event - MAX_BaseClientCommunication_Event][25] =
{"Init", "Connection", "Disconnection", "Re Connexion"};

#endif