#include "Timer.h"
#include "Timer_Delegate.h"

#if defined (WIN32)
#include "Windows.h"
#pragma comment(lib, "Winmm.lib")
void CALLBACK TimerCallback(UINT uTimerID, UINT uMsg, DWORD pData, DWORD dw1, DWORD dw2);
#endif

CTimer::CTimer(TU32_t FirePeriod, CTimerDelegate * pTimerDelegate, bool StartNow, TS08_t Timer_Name)
:m_Started(false)
, m_TimerID(0)
, m_FirePeriod(FirePeriod)
, m_pTimerDelegate(pTimerDelegate)
{
	m_TimerName = Timer_Name;
	if(StartNow)
		Start();
}
CTimer::~CTimer()
{
	Stop();
}
bool CTimer::Start()
{
	if(m_Started || !m_pTimerDelegate)
		return false;

#if defined (WIN32)
	if(!m_TimerID)
	{
		m_TimerID = timeSetEvent(m_FirePeriod, 10, TimerCallback, (DWORD)this, TIME_PERIODIC);

		if(m_TimerID)
			m_Started = true;
	}
#endif

	return m_Started;
}
bool CTimer::Stop()
{
	if(!m_Started)
		return false;

#if defined (WIN32)
	if(m_TimerID)
		m_Started = (timeKillEvent(m_TimerID) == TIMERR_NOERROR);
#endif

	return !m_Started;
}
#if defined (WIN32)
void CALLBACK TimerCallback(UINT uTimerID, UINT uMsg, DWORD pData, DWORD dw1, DWORD dw2)
{
	if(pData)
	{
		CTimer * pTHIS = (CTimer *)pData;
		if(pTHIS->TimerDelegate())
			pTHIS->TimerDelegate()->TimerCallBack(uTimerID, pTHIS->TimerName());
	}
}
#endif