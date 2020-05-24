#ifndef CTIMER_H
#define CTIMER_H
#include "Common\MyTypes.h"
#include "MyString\MyString.h"

class CTimerDelegate;

class CTimer
{
public:
	CTimer(TU32_t FirePeriod, CTimerDelegate * pTimerDelegate, bool StartNow = false, TS08_t Timer_Name = NULL);
	~CTimer();
	
	bool Start();
	bool Stop();

	inline bool Started() const {return m_Started;}
	inline TU32_t FirePeriod() const {return m_FirePeriod;}
	inline TU32_t TimerID() const {return m_TimerID;}
	inline CTimerDelegate * TimerDelegate() const {return m_pTimerDelegate;}
	inline const CMyString<char> & TimerName()const {return m_TimerName;}

private:
	bool m_Started;
	TU32_t m_FirePeriod;
	TU32_t m_TimerID;
	CTimerDelegate * m_pTimerDelegate;
	CMyString<char> m_TimerName;
};
#endif