#ifndef CTHREAD_H
#define CTHREAD_H
#include "Common\MyTypes.h"
#if defined (WIN32) || defined (WINCE)
#include "Windows.h"
#else
#include "pThread.h"
#endif

/**********************************************************************************************//**
 * \typedef	void (*CThreadDelegate_t)(void *)
 *
 * \brief	Defines an alias representing the *.
 **************************************************************************************************/

typedef void (*CThreadDelegate_t)(void *);

/**********************************************************************************************//**
 * \enum	CThread_State
 *
 * \brief	Values that represent CThread_State.
 **************************************************************************************************/

typedef enum CThread_State
{
	CThread_State_Running = 0,
	CThread_State_Stop_Request,
	CThread_State_Suspend_Request,
	CThread_State_Abort_Request,
	CThread_State_UnStarted,
	CThread_State_Stopped,
	CThread_State_Suspended,
	CThread_State_Aborted,
	MAX_THREAD_STATE
}CThread_State_t;   ///< .
const TC08_t CThread_State_STR_TAB[MAX_THREAD_STATE][30] =	///< .
{"Running","Stop Request","Suspend Request","Abort Request","UnStarted","Stopped","Suspended","Abort"};

/**********************************************************************************************//**
 * \enum	CThread_Priority
 *
 * \brief	Values that represent CThread_Priority.
 **************************************************************************************************/

typedef enum CThread_Priority
{
	CThread_Priority_TIME_CRITICAL,
	CThread_Priority_HIGHEST,	
	CThread_Priority_ABOVE_NORMAL,
	CThread_Priority_NORMAL,
	CThread_Priority_BELOW_NORMAL,
	CThread_Priority_LOWEST,
	CThread_Priority_ABOVEIDLE,
	CThread_Priority_IDLE,
	MAX_THREAD_PRIORITY
}CThread_Priority_t;	///< .

const TC08_t CThread_Priority_STR_TAB[MAX_THREAD_PRIORITY][30] =  ///< .
{"TIME CRITICAL", "HIGHEST","ABOVE NORMAL","NORMAL","BELOW NORMAL","LOWEST","ABOVE IDLE","IDLE"};
const TC08_t CThread_Priority_TAB[MAX_THREAD_PRIORITY] =  ///< .
#if defined (UNDER_CE)
{THREAD_PRIORITY_TIME_CRITICAL  ///< .
, THREAD_PRIORITY_HIGHEST
, THREAD_PRIORITY_ABOVE_NORMAL
, THREAD_PRIORITY_NORMAL
, THREAD_PRIORITY_BELOW_NORMAL
, THREAD_PRIORITY_LOWEST
, THREAD_PRIORITY_ABOVE_IDLE
, THREAD_PRIORITY_IDLE};
#else
{THREAD_PRIORITY_TIME_CRITICAL  ///< .
, THREAD_PRIORITY_HIGHEST
, THREAD_PRIORITY_ABOVE_NORMAL
, THREAD_PRIORITY_NORMAL
, THREAD_PRIORITY_BELOW_NORMAL
, THREAD_PRIORITY_LOWEST
, THREAD_PRIORITY_IDLE
, THREAD_PRIORITY_IDLE};
#endif

class CLog;
class CThread
{
public:

	/**********************************************************************************************//**
	 * \fn	CThread::CThread(TS08_t ThreadName , CThreadDelegate_t ThreadFunction ,
	 * 		CLog *pLog = NULL , CThread_Priority_t Priority = CThread_Priority_NORMAL ,
	 * 		bool StartImediately = false);
	 *
	 * \brief	Initializes a new instance of the CThread class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	ThreadName	   	Name of the thread.
	 * \param	ThreadFunction 	The thread function.
	 * \param	pLog		   	(optional) [in,out] If non-null, the log.
	 * \param	Priority	   	(optional) the priority.
	 * \param	StartImediately	(optional) the start imediately.
	 **************************************************************************************************/

	CThread(TS08_t ThreadName
			, CThreadDelegate_t ThreadFunction
			, CLog *pLog = NULL
			, CThread_Priority_t Priority = CThread_Priority_NORMAL
			, bool StartImediately = false);

	/**********************************************************************************************//**
	 * \fn	CThread::~CThread();
	 *
	 * \brief	Finalizes an instance of the CThread class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	~CThread();

	/**********************************************************************************************//**
	 * \fn	void CThread::SetThreadFunction(CThreadDelegate_t ThreadFunction);
	 *
	 * \brief	Sets a thread function.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	ThreadFunction	The thread function.
	 **************************************************************************************************/

	void SetThreadFunction(CThreadDelegate_t ThreadFunction);

	/**********************************************************************************************//**
	 * \fn	bool CThread::SetPriority(CThread_Priority_t ThreadPriotity);
	 *
	 * \brief	Sets a priority.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	ThreadPriotity	The thread priotity.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetPriority(CThread_Priority_t ThreadPriotity);

	/**********************************************************************************************//**
	 * \fn	bool CThread::Start(void * pParams = NULL);
	 *
	 * \brief	Starts.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	pParams	(optional) [in,out] If non-null, options for controlling the operation.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool Start(void * pParams = NULL);

	/**********************************************************************************************//**
	 * \fn	bool CThread::Stop_Request();
	 *
	 * \brief	Stops a request.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool Stop_Request();

	/**********************************************************************************************//**
	 * \fn	bool CThread::Suspend();
	 *
	 * \brief	Suspends this object.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool Suspend();

	/**********************************************************************************************//**
	 * \fn	bool CThread::Resume();
	 *
	 * \brief	Resumes this object.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool Resume();

	/**********************************************************************************************//**
	 * \fn	bool CThread::Abort_Request();
	 *
	 * \brief	Determines if we can abort request.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool Abort_Request();
	//inline CThreadDelegate_t ThreadFunction(){return m_pThreadFunction;}

	/**********************************************************************************************//**
	 * \fn	inline void * CThread::ThreadParam()
	 *
	 * \brief	Gets the thread parameter.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	null if it fails, else.
	 **************************************************************************************************/

	inline void * ThreadParam(){return pThreadParam;}
	//inline CLog * Log(){return m_pLog;}

	/**********************************************************************************************//**
	 * \fn	inline CThread_Priority_t CThread::Priority()
	 *
	 * \brief	Gets the priority.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline CThread_Priority_t Priority(){return m_ThreadPriotity;}

	/**********************************************************************************************//**
	 * \fn	inline CThread_State_t CThread::State()
	 *
	 * \brief	Gets the state.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline CThread_State_t State(){return m_ThreadState;}

	/**********************************************************************************************//**
	 * \fn	inline DWORD CThread::ThreadId()
	 *
	 * \brief	Gets the thread identifier.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline DWORD ThreadId(){return m_ThreadID;}

	/**********************************************************************************************//**
	 * \fn	inline TS08_t CThread::ThreaName()
	 *
	 * \brief	Gets the threa name.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	.
	 **************************************************************************************************/

	inline TS08_t ThreaName(){return (TS08_t)&m_ThreadName;}
	//inline HANDLE ThreadEventHandle(){return m_ThreadEvent;}
private:
	TC08_t m_ThreadName [40]; ///< .

	/**********************************************************************************************//**
	 * \fn	friend void CThread::ThreadFct(void *pParam);
	 *
	 * \brief	Thread fct.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param [in,out]	pParam	If non-null, the parameter.
	 **************************************************************************************************/

	friend void ThreadFct(void *pParam);
	CThreadDelegate_t m_pThreadFunction;	///< The thread function
	CThread_State_t m_ThreadState;  ///< State of the thread
	CLog * m_pLog;  ///< The log
	HANDLE m_hThread;   ///< Handle of the thread
	CThread_Priority_t m_ThreadPriotity;	///< The thread priotity
	TU32_t m_ThreadID;	///< Identifier for the thread
	void * pThreadParam;	///< The thread parameter
	HANDLE m_ThreadEvent;   ///< Handle of the thread event

};
#endif