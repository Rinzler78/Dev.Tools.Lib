#ifndef TIMEFORCE_H
#define TIMEFORCE_H
#include "time.h"
#include "windows.h"
typedef struct tm Time_t;

time_t mktime(Time_t *tmbuff);
time_t mktime(Time_t *tmbuff, time_t _loctime_offset);

time_t time(time_t *timer);
time_t gmmktime(Time_t *tmbuff);
Time_t * localtime(const time_t *timer);
Time_t * offtime(const time_t *timer, long tzoffset);
Time_t * gmtime(const time_t *timer);

#endif