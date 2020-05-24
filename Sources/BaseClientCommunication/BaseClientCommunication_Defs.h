#ifndef CBASECLIENTCOMMUNICATION_DEFS_H
#define CBASECLIENTCOMMUNICATION_DEFS_H
#include "Common\MyTypes.h"
#include "String.h"

/** Private Events */
typedef enum BaseClientCommunication_Event
{
	BaseClientCommunication_Event_Init = 0,
	BaseClientCommunication_Event_Exit,
	BaseClientCommunication_Event_Send,
	BaseClientCommunication_Event_Receive,
	BaseClientCommunication_Event_Sleep,
	MAX_BaseClientCommunication_Event

}BaseClientCommunication_Event_t;

static const char BaseClientCommunication_Event_STR_TAB[MAX_BaseClientCommunication_Event][25]=
{"Init", "Send Request", "Receive Request", "Sleep"};

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