#ifndef CLOG_CPP
#define CLOG_CPP
#include "Log\Log.h"
#include "CommonTools\CommonTools.h"
#include "Thread\Thread.h"
#include "Mutex\Mutex.h"
#include "FileInfo\FileInfo.h"
#include "Clock\Clock.h"
#include "MyString\MyString.h"

#if defined __cplusplus_cli
#using <System.dll>
using namespace System::IO;
using namespace System::Media;
#endif
#define LOG_WRITE_DELAYED
//#define XML_TAG

CLog::CLog()  
: m_LogActivated(false)
, m_LogOutput(0)
, m_DateAtLineBegin(false)
, m_pFileInfo(NULL)
, m_AppNameOnline(false)
, m_pOutCallBack(NULL)
, m_pLogListAccessMutex(NULL)
, m_hEventNewLogInList(NULL)
, m_pLogVarlistMutex(NULL)
, m_pThreadLog(NULL)
, m_pAppName(NULL)
{
	Initialise();
}
CLog::CLog(const TS08_t pPathAndFilename)  
: m_LogActivated(false)
, m_LogOutput(0)
, m_DateAtLineBegin(false)
, m_pFileInfo(NULL)
, m_AppNameOnline(false)
, m_pOutCallBack(NULL)
, m_pLogListAccessMutex(NULL)
, m_hEventNewLogInList(NULL)
, m_pLogVarlistMutex(NULL)
, m_pThreadLog(NULL)
, m_pAppName(NULL)
{
	Initialise();
	SetPathAndFileName(pPathAndFilename);
}
CLog::~CLog() 
{
#if !defined __cplusplus_cli
	SetEvent(m_hEventNewLogInList);
	MACRO_DELET(m_pThreadLog)
	if(m_hEventNewLogInList)
		CloseHandle(m_hEventNewLogInList);
	MACRO_DELET(m_pLogListAccessMutex)
	MACRO_DELET(m_pLogVarlistMutex)
	MACRO_DELET(m_pFileInfo)
	MACRO_DELET(m_pAppName)
#endif
}
void CLog::Initialise()
{
	if(!m_hEventNewLogInList)
		m_hEventNewLogInList = CreateEvent(NULL,false,false,NULL);
	if(!m_pLogListAccessMutex)
		m_pLogListAccessMutex = new CMutex(L"Log List Access");
	if(!m_pLogVarlistMutex)
		m_pLogVarlistMutex = new CMutex(L"Log Var List Mutex");
	if(!m_pThreadLog)
	{	
		m_pThreadLog = new CThread("Log thread", LogThreadFunction, NULL,CThread_Priority_BELOW_NORMAL);
		m_pThreadLog->Start(this);
	}
}
#if defined __cplusplus_cli
bool CLog::SetPathAndFileName(const TS08_t pPathAndFilename)
#else
bool CLog::SetPathAndFileName(const TS08_t pPathAndFilename,...)
#endif
{
	bool Result = false;
	
	va_list vl = NULL;
	
	if(pPathAndFilename)
	{
		va_start( vl, pPathAndFilename );
		CMyString<char> & String_PathAndFileName = CMyString<char>::GetVarList(vl, pPathAndFilename);
		va_end(vl);

		m_pFileInfo = new CFileInfo(String_PathAndFileName);
	}

	return Result;
}
TS08_t CLog::FileName()
{
	return (m_pFileInfo ? m_pFileInfo->FullName().String() : NULL);
}
#if defined __cplusplus_cli
void CLog::Log(Log_type_t LogType, const TS08_t  pComments)
#else
void CLog::Log(Log_type_t LogType, const TS08_t  pComments, ...)
#endif
{
	CMyString<char> LogLine;
	va_list vl = NULL;
	
	/** Play sound Alerte depend on LogType */
	SoundOnLog(LogType);

	if(m_LogActivated && pComments)
	{
#if defined XML_TAG
		//sprintf(LogLine + LogLineBufferpos,"<LogLine>");
		//LogLineBufferpos = strlen(LogLine);
		LogLine.AddString("%s", "<LogLine>");
#endif

		if(m_DateAtLineBegin)
		{
			static CMyString<char> DatetimeNow = DateTimeNow();
#if defined XML_TAG
			LogLine.AddString("<DateTime>%s</DateTime>", pDatetimeNow->String());
#else
			LogLine.AddString("%s ", DatetimeNow.String());
#endif
		}
		if(m_AppNameOnline)
		{
#if defined XML_TAG
			LogLine.AddString("<AppName>%s</AppName>", m_pAppName->String());
#else
			LogLine.AddString("%s ", m_pAppName->String());
#endif
		}
		/** Log Type */
		if(LogType != NOCOMMENTS)
		{
#if defined XML_TAG
			LogLine.AddString("<Type>%s</Type>", Log_type_CharTab[LogType]);
#else
			LogLine.AddString("%s ", Log_type_CharTab[LogType]);
#endif
		}

		va_start(vl, pComments);
		CMyString<char> VarListString = CMyString<char>::GetVarList(vl, pComments);
		va_end(vl);

#if defined XML_TAG
		LogLine.AddString("<Log>%s</Log></LogLine>", pVarListString->String());
#else
		LogLine += VarListString;
#endif

#if defined (LOG_WRITE_DELAYED)
		LogDelayed(LogLine);
#else
		Log(LogLine);
#endif
	}
}
void CLog::Log(const CMyString<char> & LogString)
{
	if(m_LogActivated)
	{
		/** Out Put */
		if(m_LogOutput & LOG_OUT_FILE)
		{
			/** Write on File Log */
			if(!m_pFileInfo)
			{
				SetPathAndFileName(	"%s%s"
									, CFileSystemInfo::GetCurrent_AppFullPath().String()
									,".txt");
			}
			if(m_pFileInfo)
				m_pFileInfo->WriteLine(LogString.String());
		}
		if(m_LogOutput & LOG_OUT_CONSOLE)
		{
			/** Write On Console */
			LogToConsole(LogString.String());
		}
		if(m_LogOutput & LOG_OUT_PRINT)
		{
			/** Write On Printer */
			LogToPrinter(LogString.String());
		}
		if(m_LogOutput & LOG_OUT_SPECIFIED_OUT)
		{
			if(m_pOutCallBack)
				m_pOutCallBack(m_pToObj, LogString.String(), LogString.Length());
		}
	}
}
bool CLog::LogToConsole(const TS08_t  pComments)
{
	bool Result = false;
	
	if(pComments)
	{
		if(printf("%s\n",pComments))
			Result = true;	
	}

	return Result;
}
bool CLog::LogToPrinter(const TS08_t pComments)
{
	bool Result = false;

	if(pComments)
	{
		
	}

	return Result;
}
bool CLog::SoundOnLog(Log_type_t LogType)
{
	bool Result = false;
	
	switch(LogType)
	{
		case NOCOMMENTS:
			break;
		case NORMAL_USE:
			break;
		case ALERT:
#if defined __cplusplus_cli
			SystemSounds::Beep->Play();
#else
			MessageBeep(MB_ICONERROR);
			MessageBeep(MB_ICONERROR);
#endif
			break;
		case ERROR_Log:
#if defined __cplusplus_cli
			SystemSounds::Asterisk->Play();
#else
			MessageBeep(MB_ICONERROR);
#endif
			break;
		default:
			break;
	}
	return Result;
}
void CLog::Active_Debug(bool NewFile, TU08_t LogOut, bool DateAtLineBegin, bool AppNameLineBegin)
{
	/** Open File in write -> Erase File Content */
	if(NewFile)
	{
		if(!m_pFileInfo)
			SetPathAndFileName(	"%s%s"
										, CFileSystemInfo::GetCurrent_AppFullPath().String()
										,".txt");
		if(m_pFileInfo)
			m_pFileInfo->Delete();
	}
		

	/** Define Log Output */
	m_LogOutput |= LogOut;

	/** Date At Line Begin */
	m_DateAtLineBegin = DateAtLineBegin;

	m_AppNameOnline = AppNameLineBegin;

	if(m_AppNameOnline)
		m_pAppName = new CMyString<char>(CFileSystemInfo::GetCurrent_AppName());
	
	m_LogActivated = true;
}
CMyString<char> CLog::DateTimeNow()
{
	CMyString<char> Result;

	TC08_t DateTimeSTR [200];
	SYSTEMTIME TimeStruct = {0};

	/** Init Static Char Buffer */
	MACRO_MEMSET(DateTimeSTR, sizeof(DateTimeSTR))
	
	/** get Current Time */
	GetSystemTime(&TimeStruct);

	/** Construct String */
	sprintf_s(	DateTimeSTR
				, sizeof(DateTimeSTR)
				, "%02d/%02d/%04d %02d:%02d:%02d"
				, TimeStruct.wDay
				, TimeStruct.wMonth
				, TimeStruct.wYear
				, TimeStruct.wHour
				, TimeStruct.wMinute
				, TimeStruct.wSecond);

	Result = DateTimeSTR;

	return Result;
}
void CLog::AddSpecialOutput(void *pToObj, pSpecifiedOutCallBack OutPutCallBack)
{
	if(OutPutCallBack)
	{
		m_pOutCallBack = OutPutCallBack;
		m_pToObj = pToObj;
		AddLogOutput(LOG_OUT_SPECIFIED_OUT);
	}
}
void CLog::LogDelayed(const CMyString<char> & LogString)
{
	m_pLogListAccessMutex->Wait();
	m_LogList.Add(new CMyString<char>(LogString));
	m_pLogListAccessMutex->Release();
	SetEvent(m_hEventNewLogInList);
}

void LogThreadFunction(void *pToObj)
{
	CThread * pThread = (CThread *)pToObj;
	CLog * pThis = (CLog *)pThread->ThreadParam();

	while((pThread->State() == CThread_State_Running) || (pThis->m_LogList.Count() > 0))
	{
		WaitForSingleObject(pThis->m_hEventNewLogInList,INFINITE);
		
		if(pThis->m_LogList.Count() > 0)
		{
			if(pThis->m_pFileInfo)
				pThis->m_pFileInfo->LockAutoClose();

			CMyString<char> * pLog = NULL;
			while(pThis->m_LogList.Count())
			{
				pThis->m_pLogListAccessMutex->Wait();
				pLog = pThis->m_LogList[0];
				pThis->m_pLogListAccessMutex->Release();
				if(!pLog)
					continue;

				pThis->Log(*pLog);
				
				pThis->m_pLogListAccessMutex->Wait();
				pThis->m_LogList.DeleteAt(0);
				MACRO_DELET(pLog)
				pThis->m_pLogListAccessMutex->Release();
			}
			if(pThis->m_pFileInfo)
				pThis->m_pFileInfo->Close();
		}
	}
}
#endif



