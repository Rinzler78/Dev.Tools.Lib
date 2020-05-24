#include "Mutex\Mutex.h"
#if ! defined (WIN32)
#include <pthread.h>
#endif

#if defined (WIN32) || defined (WINCE)
#include <windows.h>
static const TU32_t MutexWait_Val_Tab[MAX_WAIT_STATUS] = {WAIT_OBJECT_0, WAIT_TIMEOUT, WAIT_FAILED};
#else
static const TU32_t MutexWait_Val_Tab[MAX_WAIT_STATUS] = {0, 1, 2};
#endif

CMutex::CMutex(const TC16_t * pMutexName)
:m_MutexLocked(false)
#if defined (WIN32) || defined (WINCE)
, m_hMutexHandle(NULL)
#endif
{
#if defined (WIN32) || defined (WINCE)
	m_hMutexHandle = CreateMutex(NULL
					,FALSE
					,pMutexName);
#else
	pthread_mutex_init(&m_hMutexHandle, NULL);
#endif
}
CMutex::~CMutex()
{
	/** Release Mutex before destruction */
	Release();

	/** Mutex Destruction */
#if defined (WIN32) || defined (WINCE)
	if(m_hMutexHandle)
	{
		CloseHandle(m_hMutexHandle);
		m_hMutexHandle = NULL;
	}
#endif
}
#if defined (WIN32) || defined (WINCE)
MutexWaitStatus_t CMutex::Wait(TU32_t MaxWaitingTime)
#else
MutexWaitStatus_t CMutex::Wait()
#endif
{
	MutexWaitStatus_t Result = MutexWait_Failure;
	
#if defined (WIN32) || defined (WINCE)
	if(m_hMutexHandle && MaxWaitingTime > 0)
	{
		TU32_t Answer = WaitForSingleObject (m_hMutexHandle, MaxWaitingTime);

		switch(Answer)
		{
			case WAIT_OBJECT_0:
				Result = MutexWait_Success;
				m_MutexLocked = true;
				break;
			case WAIT_TIMEOUT:
				Result = MutexWait_TimeOut;
				break;
			case WAIT_FAILED:
				Result = MutexWait_Failure;
				break;
			default:
				break;
		}
	}
#else
	pthread_mutex_lock(&m_hMutexHandle);
#endif

	return Result;
}
bool CMutex::Release()
{
	bool Result = false;
	
	#if defined (WIN32) || defined (WINCE)
	if(m_hMutexHandle)
	{
		Result = (ReleaseMutex(m_hMutexHandle) == TRUE);
		m_MutexLocked = !Result;
	}
#else
	pthread_mutex_unlock(&m_hMutexHandle);
#endif

	return Result;
}