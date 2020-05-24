#ifndef CCLOCK_H
#define CCLOCK_H
#include "Clock_defs.h"
#include "time.h"
typedef struct tm Time_t;

#if defined (WIN32) || defined (WINCE)
#include "windows.h"
#endif

class CClock
{
public:
	CClock();
	CClock(const CClock &);
	CClock(	const TU16_t & Year
			, const CClock_Months_t & Month
			, const CClock_Days_t & WeekDay
			, const TU08_t & MonthDay
			, const TU16_t & YearDay
			, const TU08_t & Hour
			, const TU08_t & Minutes
			, const TU08_t & Secondes
			, const TU16_t & MilliSecondes);
	~CClock();
	void operator = (const CClock &);

	static CClock GetLocalTime();
	static CClock GetElapsedTime(const CClock & LaterClock, const CClock & EarlierClock);
	static CClock GetFromTimeStructure(const Time_t &);
#if defined (WIN32) || defined (WINCE)|| defined (WINCE)
	static CClock GetFromTSystemTime(SYSTEMTIME &) ;
#endif
	CClock GetElapsedTime(const CClock & LaterClock);

	/** Date */
	inline TU16_t Year() const{return m_Year;}
	inline CClock_Months_t Month()const{return m_Month;}
	inline CClock_Days_t WeekDay()const{return m_WeekDay;}
	inline TU08_t MonthDay()const{return m_MonthDay;}
	inline TU16_t YearDay()const{return m_YearDay;}
	
	/** Time */
	inline TU08_t Hour()const{return m_Hour;}
	inline TU08_t Minutes()const{return m_Minutes;}
	inline TU08_t Secondes()const{return m_Secondes;}
	inline TU16_t MilliSecondes()const{return m_MilliSecondes;}

	/** Convertion */
	Time_t ToTimeStructure() const;

private:
	/** Date */
	TU16_t m_Year;
	CClock_Months_t m_Month;
	CClock_Days_t m_WeekDay;
	TU08_t m_MonthDay;
	TU16_t m_YearDay;

	/** Time */
	TU08_t m_Hour;
	TU08_t m_Minutes;
	TU08_t m_Secondes;
	TU16_t m_MilliSecondes;
};
#endif