#include "TimeForCE.h"
#define MONTHS_NUMBER 12
static const int MONTHDAYS[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
#define SECS_PER_MIN	60
#define MINS_PER_HOUR	60
#define HOURS_PER_DAY	24
#define DAYS_PER_WEEK	7
#define DAYS_PER_NYEAR	365
#define DAYS_PER_LYEAR	366
#define SECS_PER_HOUR	(SECS_PER_MIN * MINS_PER_HOUR)
#define SECS_PER_DAY	((long) SECS_PER_HOUR * HOURS_PER_DAY)
#define MONS_PER_YEAR	12

#define TM_SUNDAY	0
#define TM_MONDAY	1
#define TM_TUESDAY	2
#define TM_WEDNESDAY	3
#define TM_THURSDAY	4
#define TM_FRIDAY	5
#define TM_SATURDAY	6

#define TM_JANUARY	0
#define TM_FEBRUARY	1
#define TM_MARCH	2
#define TM_APRIL	3
#define TM_MAY		4
#define TM_JUNE		5
#define TM_JULY		6
#define TM_AUGUST	7
#define TM_SEPTEMBER	8
#define TM_OCTOBER	9
#define TM_NOVEBER	10
#define TM_DECEMBER	11
#define TM_SUNDAY	0

#define TM_YEAR_BASE	1900

#define EPOCH_YEAR	1970
#define EPOCH_WDAY	TM_THURSDAY

#define isleap(y) (((y) % 4) == 0 && ((y) % 100) != 0 || ((y) % 400) == 0)

time_t time(time_t *timer)
{
    time_t t;
    struct tm tmbuff;
    SYSTEMTIME st;

    /* Retrive current system date time as UTC */
    GetSystemTime(&st);

    /* Build tm struct based on SYSTEMTIME values */

    /* Date values */
    tmbuff.tm_year = st.wYear - TM_YEAR_BASE;
    tmbuff.tm_mon = st.wMonth - 1;      /* wMonth value 1-12 */
    tmbuff.tm_mday = st.wDay;

    /* Time values */
    tmbuff.tm_hour = st.wHour;
    tmbuff.tm_min = st.wMinute;
    tmbuff.tm_sec = st.wSecond;
    tmbuff.tm_isdst = 0;    /* Always 0 for UTC time. */
    tmbuff.tm_wday = st.wDayOfWeek;
    tmbuff.tm_yday = 0;     /* Value is set by wceex_gmmktime */

    /* Convert tm struct to time_tUTC */
    t = gmmktime(&tmbuff);

    /* Assign time value. */
    if (timer != NULL)
    {
        *timer = t;
    }

	return t;
}
time_t mktime(Time_t *tmbuff)
{
    time_t offset;
    TIME_ZONE_INFORMATION tzi;
    
    offset = 0;

    // Retrive timezone offset in seconds
	if (GetTimeZoneInformation(&tzi) != 0xFFFFFFFF)
	{
		offset += (tzi.Bias * 60);
		if (tzi.StandardDate.wMonth != 0)
        {
			offset += (tzi.StandardBias * 60);
        }
	}

    return mktime(tmbuff, offset);
}
time_t gmmktime(Time_t *tmbuff)
{
    return mktime(tmbuff, 0);
}
time_t mktime(Time_t *tmbuff, time_t _loctime_offset)
{
    time_t tres;
    int doy;
    int i;

    /* We do allow some ill-formed dates, but we don't do anything special
    with them and our callers really shouldn't pass them to us.  Do
    explicitly disallow the ones that would cause invalid array accesses
    or other algorithm problems. */
    if (tmbuff->tm_mon < 0 || tmbuff->tm_mon > 11 || tmbuff->tm_year < (EPOCH_YEAR - TM_YEAR_BASE))
    {
        return (time_t) -1;
    }

    /* Convert calender time to a time_t value. */
    tres = 0;

    /* Sum total amount of days from the Epoch with respect to leap years. */
    for (i = EPOCH_YEAR; i < tmbuff->tm_year + TM_YEAR_BASE; i++)
    {
        tres += 365 + isleap(i);
    }

    /* Add days of months before current month. */
    doy = 0;
    for (i = 0; i < tmbuff->tm_mon; i++)
    {
        doy += MONTHDAYS[i];
    }
    tres += doy;
    
    /* Day of year */
    tmbuff->tm_yday = doy + tmbuff->tm_mday;

    if (tmbuff->tm_mon > 1 && isleap(tmbuff->tm_year + TM_YEAR_BASE))
    {
        tres++;
    }
    
    /* Add days of current month and convert to total to hours. */
    tres = 24 * (tres + tmbuff->tm_mday - 1) + tmbuff->tm_hour;

    /* Add minutes part and convert total to minutes. */
    tres = 60 * tres + tmbuff->tm_min;

    /* Add seconds part and convert total to seconds. */
    tres = 60 * tres + tmbuff->tm_sec;
    
    /* For offset > 0 adjust time value for timezone
    given as local to UTC time difference in seconds). */
    tres += _loctime_offset;
    
    return tres;
}


Time_t * localtime(const time_t *timer)
{
    register Time_t *tmp;

    long tzoffset;
    TIME_ZONE_INFORMATION tzi;
    
    // Retrive timezone offset in seconds
    tzoffset = 0;
	if (GetTimeZoneInformation(&tzi) != 0xFFFFFFFF)
	{
		tzoffset += (tzi.Bias * 60);
		if (tzi.StandardDate.wMonth != 0)
        {
			tzoffset += (tzi.StandardBias * 60);
        }
	}
    
    tzoffset *= -1;
    tmp = offtime(timer, tzoffset);

    return tmp;
}

Time_t * gmtime(const time_t *timer)
{
    register Time_t *tmp;

    tmp = offtime(timer, 0L);

    return tmp;
}

/*******************************************************************************
* __wceex_offtime - Convert time_t value to tm struct.
*
* Description:
*   Use offset as difference in seconds between local time and UTC time.
*
*******************************************************************************/
static int	mon_lengths[2][MONS_PER_YEAR] =
{
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
    31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static int	year_lengths[2] = { DAYS_PER_NYEAR, DAYS_PER_LYEAR };


Time_t * offtime(const time_t *timer, long tzoffset)
{
    register Time_t *tmp;
    register long		days;
    register long		rem;
    register int		y;
    register int		yleap;
    register int       *ip;
    static Time_t    tm;

    tmp = &tm;
    days = *timer / SECS_PER_DAY;
    rem = *timer % SECS_PER_DAY;
    rem += tzoffset;
    while (rem < 0)
    {
        rem += SECS_PER_DAY;
        --days;
    }
    
    while (rem >= SECS_PER_DAY)
    {
        rem -= SECS_PER_DAY;
        ++days;
    }
    tmp->tm_hour = (int) (rem / SECS_PER_HOUR);

    rem = rem % SECS_PER_HOUR;
    tmp->tm_min = (int) (rem / SECS_PER_MIN);
    tmp->tm_sec = (int) (rem % SECS_PER_MIN);
    tmp->tm_wday = (int) ((EPOCH_WDAY + days) % DAYS_PER_WEEK);
    
    if (tmp->tm_wday < 0)
        tmp->tm_wday += DAYS_PER_WEEK;
    
    y = EPOCH_YEAR;
    
    if (days >= 0)
    {
        for ( ; ; )
        {
            yleap = isleap(y);
            if (days < (long) year_lengths[yleap])
                break;

            ++y;
            days = days - (long) year_lengths[yleap];
        }
    }
    else
    {
        do
        {
            --y;
            yleap = isleap(y);
            days = days + (long) year_lengths[yleap];
        } while (days < 0);
    }

    tmp->tm_year = y - TM_YEAR_BASE;
    tmp->tm_yday = (int) days;
    ip = mon_lengths[yleap];

    for (tmp->tm_mon = 0; days >= (long) ip[tmp->tm_mon]; ++(tmp->tm_mon))
    {
        days = days - (long) ip[tmp->tm_mon];
    }

    tmp->tm_mday = (int) (days + 1);
    tmp->tm_isdst = 0;

    return tmp;
}