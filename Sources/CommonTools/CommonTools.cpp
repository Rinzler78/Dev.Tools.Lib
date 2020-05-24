#ifndef CCOMMON_TOOLS_CPP
#define CCOMMON_TOOLS_CPP
#include "CommonTools\CommonTools.h"
#include "LinkedList\LinkedList.h"
#if defined __cplusplus_cli
/** launch program */
#using <System.dll>
using namespace System;
using namespace System::Diagnostics;
using namespace System::ComponentModel;
#else
#include "windows.h"
#endif

#define FULLNAME_MAX_NB_LETTER 100
#define FILENAME_MAX_NB_LETTER 50
//#define CREATEPROC_METHODE
#define SHELLEXECUTE_METHODE

bool CCommonTools::Launchprogram(const TS08_t  pPathAndFileName, bool WaitUntilExit)
{
	TU32_t ExitCode = 0;
	return Launchprogram(pPathAndFileName, ExitCode, WaitUntilExit);
}
bool CCommonTools::Launchprogram(const TS08_t  pPathAndFileName, TU32_t &ExitCode, bool WaitUntilExit)
{ 
	bool Result = false;

	TS16_t pWPathAndFileName = NULL;
	TU32_t Size = 0;
	
	if(pPathAndFileName)
	{
		Size = strlen(pPathAndFileName);
		pWPathAndFileName = new TC16_t[Size + 1];

		/** Convert TC08_t string to wchar string */
		MACRO_MEMSET(pWPathAndFileName, Size + 1)
		mbstowcs(pWPathAndFileName,pPathAndFileName, Size); 

		/** Launche program */
		Result = Launchprogram(pWPathAndFileName, ExitCode, NULL, WaitUntilExit);
	}

	MACRO_DELET_TAB(pWPathAndFileName);

	return Result;
}
bool CCommonTools::Launchprogram(const TS16_t pPathAndFileName, const TS16_t pArguments, bool WaitUntilExit)
{
	TU32_t ExitCode = 0;
	return Launchprogram(pPathAndFileName, ExitCode, pArguments, WaitUntilExit);
}
bool CCommonTools::Launchprogram(const TS16_t pPathAndFileName, TU32_t &ExitCode, const TS16_t pArguments, bool WaitUntilExit)
{
	bool Result = false;
#if defined __cplusplus_cli
	Process ^ProcessToLaunch = gcnew Process();
	ProcessToLaunch->StartInfo->FileName = pPathAndFileName;
	ProcessToLaunch->StartInfo->Arguments = pArguments;
	Result = ProcessToLaunch->Start();

	if(WaitUntilExit)
		ProcessToLaunch->WaitForExit();
#else
	DWORD dwWaitReturn = 0;

#if defined (CREATEPROC_METHODE)
	PROCESS_INFORMATION  Process = {0};
	STARTUPINFO StartupInfo = {0};
	StartupInfo.cb = sizeof(StartupInfo);
	
#elif defined (SHELLEXECUTE_METHODE)
	SHELLEXECUTEINFO sei; 
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO)); 
	sei.cbSize = sizeof(SHELLEXECUTEINFO ); 
#endif
	
	if(pPathAndFileName)
	{
		/** Launch MCT App */
#if defined (CREATEPROC_METHODE)
		Result = CreateProcess(pPathAndFileName,
							 NULL,
							 NULL,
							 NULL,
							 FALSE,
							 NULL,
							 NULL,
							 NULL,
							 &StartupInfo,
							 &Process);

		if(Result && WaitUntilExit) 
		{ 
			/** Wait until MCT finish */
			dwWaitReturn = WaitForSingleObject(Process.hProcess, INFINITE);
			
			if(dwWaitReturn != WAIT_OBJECT_0)
				Result = false;
		}

		CloseHandle(Process.hThread);
		CloseHandle(Process.hProcess);
#elif defined (SHELLEXECUTE_METHODE)
		sei.lpFile = pPathAndFileName;
		sei.lpParameters = pArguments;
		// attention ce flag est important ici !
		sei.fMask = SEE_MASK_NOCLOSEPROCESS; 
		sei.lpVerb = TEXT("open"); 
		sei.nShow = SW_SHOWNORMAL; 
		Result = (ShellExecuteEx(&sei) == TRUE);
		if(Result && WaitUntilExit) 
		{ 
			dwWaitReturn = WaitForSingleObject(sei.hProcess, INFINITE); 
			if(dwWaitReturn != WAIT_OBJECT_0)
			Result = false;
			GetExitCodeProcess(sei.hProcess, (LPDWORD)&ExitCode);
		}
		::CloseHandle(sei.hProcess);
#endif
	}
#endif

	return Result;
}
void CCommonTools::Split(const TS08_t  pCharString, const TS08_t SeperatorString, void * pObj, pFieldFoundCallBack FielFound)
{
	TU32_t FieldNumber = 0;
	TU32_t StrTSize = 0;
	TU32_t BufferPos = 0;
	TU32_t CharStrTStartPos = 0;
	TU32_t CharStringFoundedSize = 0;
	TI32_t StartSeparatorIndex = 0;
	TU32_t StrTSeparatorlength = 0;
	
	if(pCharString && SeperatorString && FielFound)
	{
		StrTSize = strlen(pCharString);
		StrTSeparatorlength = strlen(SeperatorString);
		
		if(StrTSize > 0)
		{
			while(pCharString[BufferPos])
			{
				CharStrTStartPos = BufferPos;
				StartSeparatorIndex = FindFirstIndex(&pCharString[BufferPos], SeperatorString, StrTSize);
				if(StartSeparatorIndex > -1)
				{
					/** Calculate string Size */
					CharStringFoundedSize = StartSeparatorIndex;
					BufferPos += CharStringFoundedSize + StrTSeparatorlength;
					if(!CharStringFoundedSize)
					{
						++FieldNumber;
						continue;
					}
				}
				else
				{
					/** Calculate string Size */
					CharStringFoundedSize = StrTSize - CharStrTStartPos;
					BufferPos += CharStringFoundedSize;
				}
				
				/** Allocate memory for new string */
				TS08_t pNewCharString = new TC08_t [CharStringFoundedSize + 1];
				/** Init New Buffer */
				MACRO_MEMSET(pNewCharString, CharStringFoundedSize + 1)
				/** Populate nex string */
				memcpy(pNewCharString, pCharString + CharStrTStartPos, CharStringFoundedSize);
				
				FielFound(pObj, pNewCharString, FieldNumber++);
				
				MACRO_DELET_TAB(pNewCharString);
			}
		}
	}
}
TI32_t CCommonTools::FindFirstIndex(const TS08_t  pCharString, const TS08_t SeperatorString, TU32_t MaxSize, TU32_t Offset)
{
	TI32_t Result = -1;
	TI32_t Bufferpos = 0;
	TU32_t StrTSeparatorSize = 0;

	if(pCharString)
	{
		if(SeperatorString)
		{
			StrTSeparatorSize = strlen(SeperatorString) * sizeof(*SeperatorString);
			Bufferpos = Offset;

			while(pCharString[Bufferpos] && (Bufferpos < (TI32_t)MaxSize))
			{
				if(pCharString[Bufferpos] == SeperatorString[0])
				{
					if(memcmp(&pCharString[Bufferpos],SeperatorString, StrTSeparatorSize) == 0)
					{
						//String Serator Founded
						Result = Bufferpos;
						break;
					}
				}
				++Bufferpos;
			}
		}
	}
	return Result;
}
TI32_t CCommonTools::FindFirstIndex(const TS08_t  pCharString, const TC08_t Seperator, TU32_t MaxSize, TU32_t Offset)
{
	TI32_t Result = -1;
	TI32_t Bufferpos = Offset;
	TU32_t Size = 0;

	if(pCharString)
	{
		Size = strlen(pCharString);
		if(Size)
		{
			while(pCharString[Bufferpos] && (Bufferpos < (TI32_t)MaxSize))
			{
				if((pCharString[Bufferpos] == Seperator))
				{
					Result = Bufferpos;
					break;
				}
				++Bufferpos;
			}
		}
	}
	return Result;
}
TI32_t CCommonTools::FindFirstIndex(const TS16_t  pCharString, const TS16_t SeperatorString, TU32_t MaxSize, TU32_t Offset)
{
	TI32_t Result = -1;
	TI32_t Bufferpos = 0;
	TU32_t StrTSeparatorSize = 0;

	if(pCharString)
	{
		if(SeperatorString)
		{
			StrTSeparatorSize = wcslen(SeperatorString) * sizeof(*SeperatorString);
			Bufferpos = Offset;

			while(pCharString[Bufferpos] && (Bufferpos < (TI32_t)MaxSize))
			{
				if(pCharString[Bufferpos] == SeperatorString[0])
				{
					if(memcmp(&pCharString[Bufferpos],SeperatorString, StrTSeparatorSize) == 0)
					{
						//String Serator Founded
						Result = Bufferpos;
						break;
					}
				}
				++Bufferpos;
			}
		}
	}
	return Result;
}
TI32_t CCommonTools::FindFirstIndex(const TS16_t  pCharString, const TC16_t Seperator, TU32_t MaxSize, TU32_t Offset)
{
	TI32_t Result = -1;
	TI32_t Bufferpos = 0;
	TU32_t Size = 0;

	if(pCharString)
	{
		Size = wcslen(pCharString);
		if(Size)
		{
			while(pCharString[Bufferpos] && (Bufferpos < (TI32_t)MaxSize))
			{
				if((pCharString[Bufferpos] == Seperator))
				{
					Result = Bufferpos;
					break;
				}
				++Bufferpos;
			}
		}
	}
	return Result;
}
TI32_t CCommonTools::FindLastIndex(const TS08_t  pCharString, const TC08_t Seperator, TU32_t Offset)
{
	TI32_t Result = -1;
	TI32_t Bufferpos = 0;
	TU32_t Size = 0;

	if(pCharString)
	{
		Size = strlen(pCharString);
		if(Size)
		{
			Bufferpos = Size - Offset - 1;
			while(Bufferpos)
			{
				if((pCharString[Bufferpos] == Seperator))
				{
					Result = Bufferpos;
					break;
				}
				--Bufferpos;
			}
			if(!Bufferpos && (pCharString[Bufferpos] == Seperator))
				Result = Bufferpos;
		}
	}
	return Result;
}
TI32_t CCommonTools::FindLastIndex(const TS16_t  pCharString, const TC16_t Seperator, TU32_t Offset)
{
	TI32_t Result = -1;
	TI32_t Bufferpos = 0;
	TU32_t Size = 0;

	if(pCharString)
	{
		Size = wcslen(pCharString);
		if(Size)
		{
			Bufferpos = wcslen(pCharString) - Offset - 1;
			while(Bufferpos)
			{
				if((pCharString[Bufferpos] == Seperator))
				{
					Result = Bufferpos;
					break;
				}
				--Bufferpos;
			}
			if(!Bufferpos && (pCharString[Bufferpos] == Seperator))
				Result = Bufferpos;
		}
	}
	return Result;
}
const TS08_t CCommonTools::ToBinaryString(TC08_t Byte)
{
	const TU08_t NbBits = 8;
	static TC08_t BinaryString [NbBits] = {0};
	
	MACRO_MEMSET(BinaryString, sizeof(BinaryString))

	for(TU08_t i = 0 ; i < NbBits ; ++i)
		sprintf(BinaryString + i
				,"%c"
				, 0X30 + ((Byte >> (NbBits-i)) & 1));

	return BinaryString;
}
const TS08_t CCommonTools::ToBinaryString(TU16_t Byte)
{
	const TU08_t NbBits = 16;
	static TC08_t BinaryString [NbBits] = {0};
	
	MACRO_MEMSET(BinaryString, sizeof(BinaryString))

	for(TU08_t i = 0 ; i < NbBits ; ++i)
		sprintf(BinaryString + i
				,"%c"
				, 0X30 + ((Byte >> (NbBits-i)) & 1));

	return BinaryString;
}

#endif