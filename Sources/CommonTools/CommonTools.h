#ifndef CCOMMON_TOOLS_H
#define CCOMMON_TOOLS_H
#include "Common\MyTypes.h"

TEMPLAT_DECL
class CLinkedList;

typedef void (*pFieldFoundCallBack)(void * pObj, TS08_t Field, TU32_t FieldNumber);

class CCommonTools
{
public:
	static bool Launchprogram(const TS16_t pPathAndFileName, const TS16_t pArguments = NULL, bool WaitUntilExit = false);
	static bool Launchprogram(const TS16_t pPathAndFileName, TU32_t &ExitCode, const TS16_t pArguments = NULL, bool WaitUntilExit = false);
	static bool Launchprogram(const TS08_t  pPathAndFileName, bool WaitUntilExit = false);
	static bool Launchprogram(const TS08_t  pPathAndFileName, TU32_t &ExitCode, bool WaitUntilExit = false);

	static void Split(const TS08_t  pCharString, const TS08_t SeperatorString, void * pObj, pFieldFoundCallBack FielFound);
	static TI32_t FindFirstIndex(const TS08_t  pCharString, const TC08_t Seperator, TU32_t MaxSize, TU32_t Offset = 0);
	static TI32_t FindFirstIndex(const TS16_t  pCharString, const TC16_t Seperator, TU32_t MaxSize, TU32_t Offset = 0);
	static TI32_t FindFirstIndex(const TS08_t  pCharString, const TS08_t SeperatorString, TU32_t MaxSize, TU32_t Offset = 0);
	static TI32_t FindFirstIndex(const TS16_t  pCharString, const TS16_t SeperatorString, TU32_t MaxSize, TU32_t Offset = 0);
	static TI32_t FindLastIndex(const TS08_t  pCharString, const TC08_t Seperator, TU32_t Offset = 0);
	static TI32_t FindLastIndex(const TS16_t  pCharString, const TC16_t Seperator, TU32_t Offset = 0);
	
	static const TS08_t ToBinaryString(TC08_t Byte);
	static const TS08_t ToBinaryString(TU16_t Byte);
};
#endif