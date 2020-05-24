#include "Thread\Thread.h"
#include "string.h"
#include "Log\Log.h"
#include "ThreadManager\ThreadManager.h"

CThread::CThread(TS08_t ThreadName, CThreadDelegate_t ThreadFunction, CLog *pLog, CThread_Priority_t Priority, bool StartImediately)
:m_pThreadFunction(ThreadFunction)
, m_pLog(pLog)
, m_hThread(0)
, m_ThreadPriotity(CThread_Priority_IDLE)
, m_ThreadID(0)
, m_ThreadState(CThread_State_UnStarted)
{
	MACRO_MEMSET(m_ThreadName, sizeof(m_ThreadName));
	if(ThreadName)
		strcpy(m_ThreadName, ThreadName);

	m_ThreadPriotity = Priority;
	m_ThreadEvent = CreateEvent(NULL,false,false,NULL);
	m_hThread = CreateThread(	NULL
								, NULL
								, (LPTHREAD_START_ROUTINE)(ThreadFct)
								, this
								, (StartImediately ? m_ThreadPriotity : CREATE_SUSPENDED)
								, (LPDWORD)&m_ThreadID);
}
CThread::~CThread()
{
	Stop_Request();

	if(m_ThreadEvent)
	{
		CloseHandle(m_ThreadEvent);
		m_ThreadEvent = NULL;
	}
}
void CThread::SetThreadFunction(CThreadDelegate_t ThreadFunction)
{
	if(m_pLog)
		m_pLog->Log(NOCOMMENTS, "Set Thread Function");
	if(!m_pThreadFunction)
	{
		m_pThreadFunction = ThreadFunction;
	}
}
bool CThread::SetPriority(CThread_Priority_t ThreadPriotity)
{
	bool Result = false;
	
	Result = (SetThreadPriority(m_hThread, CThread_Priority_TAB[ThreadPriotity]) == TRUE);
	if(!Result)
	{
		if(m_pLog)m_pLog->Log(ALERT, "Set Thread Priority to %s Failure",CThread_Priority_STR_TAB[ThreadPriotity]);
	}
	else
	{
		m_ThreadPriotity = ThreadPriotity;
		if(m_pLog)m_pLog->Log(NOCOMMENTS, "Set Thread Priority to %s Success",CThread_Priority_STR_TAB[ThreadPriotity]);
	}

	return Result;
}
bool CThread::Start(void * pParams)
{
	bool Result = false;
	pThreadParam = pParams;
	
	if((m_ThreadState == CThread_State_UnStarted))
	{
		Result = Resume();
		SetPriority(m_ThreadPriotity);
	}
	else
	{
		if(m_pLog)m_pLog->Log(NOCOMMENTS, "Cannot Start Thread (%s)-> Current State %s, Current priority %s ", m_ThreadName, CThread_State_STR_TAB[m_ThreadState], CThread_Priority_STR_TAB[m_ThreadPriotity]);
	}

	return Result;
}
bool CThread::Stop_Request()
{
	bool Result = false;
	DWORD ExitCode = 0;
	
	if(m_ThreadState != CThread_State_Stopped)
	{
		m_ThreadState = CThread_State_Stop_Request;
		
		if(m_pLog)
			m_pLog->Log(NOCOMMENTS, "Stop Thread (%s) Request.", m_ThreadName);

		WaitForSingleObject(m_ThreadEvent,INFINITE);
		//Sleep(500); /** Let the thread function finish */
		if(m_pLog)
			m_pLog->Log(NOCOMMENTS, "Thread (%s) Stoped.", m_ThreadName);
		
		m_ThreadID = 0;
		m_ThreadState = CThread_State_Stopped;
		m_ThreadPriotity = (CThread_Priority_t)GetThreadPriority(m_hThread);
		Result = (CloseHandle(m_hThread) == TRUE);
		m_hThread = NULL;
	}
	return Result;
}
bool CThread::Suspend()
{
	bool Result = false;

	if((m_ThreadState == CThread_State_Running))
	{
		if((Result = (SuspendThread(m_hThread) > 0)))
		{
			m_ThreadState = CThread_State_Suspended;
			if(m_pLog)m_pLog->Log(NOCOMMENTS, "Suspend Thread Success -> Current State %s, Current priority %s ", CThread_State_STR_TAB[m_ThreadState], CThread_Priority_STR_TAB[m_ThreadPriotity]);
		}
		else
		{
			if(m_pLog)m_pLog->Log(ALERT, "Suspend Thread failure -> Current State %s, Current priority %s ", CThread_State_STR_TAB[m_ThreadState], CThread_Priority_STR_TAB[m_ThreadPriotity]);
		}
	}
	else
	{
		if(m_pLog)m_pLog->Log(NOCOMMENTS, "Cannot Suspend Thread -> Current State %s, Current priority %s ", CThread_State_STR_TAB[m_ThreadState], CThread_Priority_STR_TAB[m_ThreadPriotity]);
	}

	return Result;
}
bool CThread::Resume()
{
	bool Result = false;

	if((m_ThreadState == CThread_State_Suspended) || (m_ThreadState == CThread_State_UnStarted))
	{
		if((Result = (ResumeThread(m_hThread) > 0)))
		{
			m_ThreadState = CThread_State_Running;
			if(m_pLog)m_pLog->Log(NOCOMMENTS, "Resume Thread (%s) Success -> Current State %s, Current priority %s ", m_ThreadName, CThread_State_STR_TAB[m_ThreadState], CThread_Priority_STR_TAB[m_ThreadPriotity]);
		}
		else
			if(m_pLog)m_pLog->Log(ALERT, "Resume Thread (%s) failure -> Current State %s, Current priority %s ", m_ThreadName, CThread_State_STR_TAB[m_ThreadState], CThread_Priority_STR_TAB[m_ThreadPriotity]);
	}
	else
		if(m_pLog)m_pLog->Log(ALERT, "Cannot Resume (%s) Thread -> Current State %s, Current priority %s ", m_ThreadName, CThread_State_STR_TAB[m_ThreadState], CThread_Priority_STR_TAB[m_ThreadPriotity]);

	return Result;
}
bool CThread::Abort_Request()
{
	bool Result = false;
	DWORD ExitCode = 0;
	
	if(m_ThreadState != CThread_State_Aborted)
	{
		m_ThreadState = CThread_State_Abort_Request;
		if(m_pLog)
			m_pLog->Log(NOCOMMENTS, "Abort Thread (%s) Request.", m_ThreadName);

		WaitForSingleObject(m_ThreadEvent,INFINITE);
		//Sleep(500); /** Let the thread function finish */

		if(m_pLog)
			m_pLog->Log(NOCOMMENTS, "Thread (%s) Aborted.", m_ThreadName);

		m_ThreadID = 0;
		m_ThreadState = CThread_State_Aborted;
		m_ThreadPriotity = (CThread_Priority_t)GetThreadPriority(m_hThread);
	}

	Result = (CloseHandle(m_hThread) == TRUE);

	return Result;
}
void ThreadFct(void *pParam)
{
	CThread *pThisThread = (CThread *)pParam;
	try
	{
		pThisThread->m_pThreadFunction(pThisThread);
	}
	catch(...)
	{
		pThisThread->m_pLog->Log(NOCOMMENTS, "Exeption leaved in %s", pThisThread->m_ThreadName);
	}
	SetEvent(pThisThread->m_ThreadEvent);
}