#ifndef CCLOCK_DEFS_H
#define CCLOCK_DEFS_H
#include "Common\MyTypes.h"

typedef enum CClock_Days
{
	CClock_Days_Init = 0,
	CClock_Days_Monday,
	CClock_Days_Tuesday,
	CClock_Days_Wesdesnday,
	CClock_Days_Thursday,
	CClock_Days_Friday,
	CClock_Days_Saturday,
	CClock_Days_Sunday,
	MAX_CClock_Days

}CClock_Days_t;

static const TC08_t CClock_Days_STR [MAX_CClock_Days][11] = 
{"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

typedef enum CClock_Months
{
	CClock_Months_Init = 0,
	CClock_Months_January,
	CClock_Months_February,
	CClock_Months_March,
	CClock_Months_April,
	CClock_Months_May,
	CClock_Months_June,
	CClock_Months_July,
	CClock_Months_August,
	CClock_Months_September,
	CClock_Months_October,
	CClock_Months_November,
	CClock_Months_December,
	MAX_CClock_Months
}CClock_Months_t;

static const TC08_t CClock_Months [MAX_CClock_Months][11] = 
{"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
#endif