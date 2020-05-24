#ifndef CMYSTRING_DEFS_H
#define CMYSTRING_DEFS_H
#include "Common\MyTypes.h"

TEMPLAT_TYPE_DECL
class CMyString;

TEMPLAT_TYPE_DECL
class CMyStringDelegate
{
public:
	virtual void FieldFound(TU32_t FiledNumber, TU32_t StartIndexInString, const CMyString<T> & StringFound) = 0;
};
#endif