#ifndef COMMON_H
#define COMMON_H

/** DLL Attributes Def */
#if defined (WIN32) || defined (WINCE)
#include <windows.h>
#endif

#define DLL_EXPORT 	__declspec(dllexport)
#define DLL_IMPORT 	__declspec(dllimport)

#include "stdio.h"
#endif
