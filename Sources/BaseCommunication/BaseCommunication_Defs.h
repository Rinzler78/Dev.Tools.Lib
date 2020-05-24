#ifndef CBASECOMMUNICATION_DEFS_H
#define CBASECOMMUNICATION_DEFS_H
#include "Common\Mytypes.h"

/** Connection State */
typedef enum BaseCommunication_State
{
	BaseCommunication_State_WaitingForFrame,
	BaseCommunication_State_FrameReceived,
	BaseCommunication_State_TryToSendFrame,
	BaseCommunication_State_FrameSent,
	MAX_BaseCommunication_State
}BaseCommunication_State_t;

static const char BaseCommunication_State_STR_TAB[MAX_BaseCommunication_State][25]= 
{
	"Waiting for Frame", "Frame Received"
	, "Try To Send Frame", "Frame Sent"
};

/** Private Events */
typedef enum BaseCommunication_Event
{
	BaseCommunication_Event_Exit,
	BaseCommunication_Event_Send,
	BaseCommunication_Event_Receive,
	BaseCommunication_Event_Sleep,
	MAX_BaseClient_Event

}BaseClient_Event_t;

static const char BaseCommunication_Event_STR_TAB[MAX_BaseClient_Event][25]=
{"Exit Event", "Send Event", "Receive Event", "Sleep Event"};

class CFrame
{
public :
	CFrame(TU08_t * pBuffer, TU32_t SizeOfBuffer, TU32_t FrameIdentifier)
	{m_pBuffer = new TU08_t[SizeOfBuffer]; memcpy(m_pBuffer, pBuffer, SizeOfBuffer); m_SizeOfBuffer = SizeOfBuffer; m_FrameIdentifier = FrameIdentifier;}
	~CFrame(){MACRO_DELET_TAB(m_pBuffer);}

	inline TU08_t * FrameBuffer()const{return m_pBuffer;}
	inline TU32_t FrameBufferSize()const{return m_SizeOfBuffer;}
	inline TU32_t FrameIdentifier()const{return m_FrameIdentifier;}
private:
	TU08_t * m_pBuffer;
	TU32_t m_SizeOfBuffer;
	TU32_t m_FrameIdentifier;
};

#endif