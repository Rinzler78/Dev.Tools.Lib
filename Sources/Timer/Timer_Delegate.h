#ifndef CTIMER_DELEGATE_H
#define CTIMER_DELEGATE_H
#include "Common\MyTypes.h"
TEMPLAT_TYPE_DECL
class CMyString;

class CTimerDelegate
{
public:
	virtual void TimerCallBack(const TU32_t & TimerID, const CMyString<char> & TimerName) = NULL;
};
#endif
