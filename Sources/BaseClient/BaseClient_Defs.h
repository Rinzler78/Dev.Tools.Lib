#ifndef CBASECLIENT_DEFS_H
#define CBASECLIENT_DEFS_H
#include "Common\MyTypes.h"
#include "string.h"

#define FIFO_IMPLEMENTATION

/** ConnectionType */
typedef enum ConnectionType
{
	ConnectionType_Serial,
	ConnectionType_IP,
	MAX_ConnectionType
}ConnectionType_t;
static const char ConnectionType_STR_TAB[MAX_ConnectionType][10] = {"Serial", "IP"};

/** Connection State */
typedef enum ClientConnectionState
{
	ClientConnectionState_TryToConnect,
	ClientConnectionState_Connected,
	ClientConnectionState_ConnectionFailure,

	ClientConnectionState_TryToDisConnect,
	ClientConnectionState_DisConnected,
	ClientConnectionState_DisConnectionFailure,

	ClientConnectionState_WaitingForFrame,
	ClientConnectionState_FrameReceived,
	ClientConnectionState_TryToSendFrame,
	ClientConnectionState_FrameSent,
	MAX_ClientConnectionState
}ClientConnectionState_t;
static const char ClientConnectionState_STR_TAB[MAX_ClientConnectionState][25]= 
{
	"Try To Connect", "Connected", "Connection Failure"
	, "Try To DisConnect", "DisConnected", "DisConnection Failure"
	, "Waiting for Frame", "Frame Received"
	, "Try To Send Frame", "Frame Sent"
};

/** Private Events */
typedef enum BaseClient_Event
{
	BaseClient_Event_Exit,
	BaseClient_Event_Connect,
	BaseClient_Event_DisConnect,
	BaseClient_Event_ReConnect,
	BaseClient_Event_Send,
	BaseClient_Event_Receive,
	BaseClient_Event_Sleep,
	MAX_BaseClient_Event

}BaseClient_Event_t;

static const char BaseClient_Event_STR_TAB[MAX_BaseClient_Event][25]=
{"Exit request", "Connection Request", "Disconnection Request", "Send Request", "Receive Request", "Sleep"};

class CFrame
{
public :
	CFrame(TU08_t * pBuffer, TU32_t SizeOfBuffer, TU32_t FrameIdentifier)
	{m_pBuffer = new TU08_t[SizeOfBuffer]; memcpy(m_pBuffer, pBuffer, SizeOfBuffer); m_SizeOfBuffer = SizeOfBuffer; m_FrameIdentifier = FrameIdentifier;}
	~CFrame(){MACRO_DELET_TAB(m_pBuffer);}

	inline TU08_t * FrameBuffer(){return m_pBuffer;}
	inline TU32_t FrameBufferSize(){return m_SizeOfBuffer;}
	inline TU32_t FrameIdentifier(){return m_FrameIdentifier;}
private:
	TU08_t * m_pBuffer;
	TU32_t m_SizeOfBuffer;
	TU32_t m_FrameIdentifier;
};
#endif