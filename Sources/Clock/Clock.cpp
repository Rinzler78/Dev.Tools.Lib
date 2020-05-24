#include "Clock\Clock.h"
#if defined (UNDER_CE)
#include "TimeForCE.h"
#endif

CClock::CClock()
:m_Year(0)
, m_Month(CClock_Months_Init)
, m_WeekDay(CClock_Days_Init)
, m_MonthDay(0)
, m_YearDay(0)
, m_Hour(0)
, m_Minutes(0)
, m_Secondes(0)
, m_MilliSecondes(0)
{
}
CClock::CClock(	const TU16_t & Year, const CClock_Months_t & Month, const CClock_Days_t & WeekDay, const TU08_t & MonthDay, const TU16_t & YearDay
	   , const TU08_t & Hour, const TU08_t & Minutes, const TU08_t & Secondes, const TU16_t & MilliSecondes)
:m_Year(Year)
, m_Month(Month)
, m_WeekDay(WeekDay)
, m_MonthDay(MonthDay)
, m_YearDay(YearDay)
, m_Hour(Hour)
, m_Minutes(Minutes)
, m_Secondes(Secondes)
, m_MilliSecondes(MilliSecondes)
{
}
CClock::CClock(const CClock & Src)
:m_Year(Src.Year())
, m_Month(Src.Month())
, m_WeekDay(Src.WeekDay())
, m_MonthDay(Src.MonthDay())
, m_YearDay(Src.YearDay())
, m_Hour(Src.Hour())
, m_Minutes(Src.Minutes())
, m_Secondes(Src.Secondes())
, m_MilliSecondes(Src.MilliSecondes())
{
}
CClock::~CClock()
{
}
void CClock::operator = (const CClock & Src)
{
	m_Year = Src.Year();
	m_Month = Src.Month();
	m_WeekDay = Src.WeekDay();
	m_MonthDay = Src.MonthDay();
	m_YearDay = Src.YearDay();
	m_Hour = Src.Hour();
	m_Minutes = Src.Minutes();
	m_Secondes = Src.Secondes();
	m_MilliSecondes = Src.MilliSecondes();
}
CClock CClock::GetFromTimeStructure(const Time_t & timeinfo) 
{
	return CClock(	timeinfo.tm_year
					, (CClock_Months_t)(timeinfo.tm_mon + 1)
					, (CClock_Days_t)(timeinfo.tm_wday + 1)
					, timeinfo.tm_mday
					, timeinfo.tm_yday + 1
					, timeinfo.tm_hour
					, timeinfo.tm_min
					, timeinfo.tm_sec
					, 0);
}
CClock CClock::GetFromTSystemTime(SYSTEMTIME & SysTime) 
{
	return CClock(	SysTime.wYear
					, (CClock_Months_t)SysTime.wMonth
					, (CClock_Days_t)SysTime.wDayOfWeek
					, (TU08_t)SysTime.wDay
					, 0
					, (TU08_t)SysTime.wHour
					, (TU08_t)SysTime.wMinute
					, (TU08_t)SysTime.wSecond
					, SysTime.wMilliseconds);
}
Time_t CClock::ToTimeStructure() const
{
	Time_t Result = {	Secondes()
						, Minutes()
						, Hour()
						, MonthDay()
						, Month() - 1
						, Year() - 1900
						, WeekDay() - 1
						, YearDay() - 1
						, 1};

	return Result;
}
CClock CClock::GetLocalTime()
{
	time_t Secondes = 0;
	Time_t * timeinfo;
	SYSTEMTIME SysT = {0};

	/** Get date And Time (Retrieve Year days value) */
	time(&Secondes);
	timeinfo = localtime(&Secondes);

	
	/** Get date And Time (Retrieve Millisecondes value) */
	GetSystemTime(&SysT);

	return CClock(	SysT.wYear
					, (CClock_Months_t) SysT.wMonth
					, (CClock_Days_t)SysT.wDayOfWeek
					, (TU08_t)SysT.wDay
					, timeinfo->tm_yday + 1
					, (TU08_t)timeinfo->tm_hour
					, (TU08_t)timeinfo->tm_min
					, (TU08_t)timeinfo->tm_sec
					, SysT.wMilliseconds);
}
CClock CClock::GetElapsedTime(const CClock & LaterClock)
{
	return GetElapsedTime(LaterClock, *this);
}
CClock CClock::GetElapsedTime(const CClock & LaterClock, const CClock & EarlierClock)
{
	CClock Result;
	time_t LaterTime = 0, EarlierTime = 0, DiffTime = 0;
	Time_t LaterTimeStruct = {0};
	Time_t EarlierTimeStruct = {0};
	TU64_t DiffInSecondes = 0;
	Time_t * pDiffTimeStruct = NULL;
	
	/** Later Date And Time */
	LaterTimeStruct = LaterClock.ToTimeStructure();
	LaterTime = mktime(&LaterTimeStruct);

	/** Earlier Date And Time */
	EarlierTimeStruct = EarlierClock.ToTimeStructure();
	EarlierTime = mktime(&EarlierTimeStruct);

	/** Calculate Diff */
	DiffTime = (time_t)difftime(EarlierTime, LaterTime);
	pDiffTimeStruct = localtime(&DiffTime);

	Result = GetFromTimeStructure(*pDiffTimeStruct);

	/** MilliSeconds */
	if(LaterClock.MilliSecondes() <= EarlierClock.MilliSecondes())
		Result.m_MilliSecondes = EarlierClock.MilliSecondes() - LaterClock.MilliSecondes();
	else
		Result.m_MilliSecondes = LaterClock.MilliSecondes() - EarlierClock.MilliSecondes();

	return Result;
}
