#ifndef CMUTEX_H
#define CMUTEX_H

#include "Common\Common.h"
#include "Common\MyTypes.h"
#if ! defined (WIN32)
#include <pthread.h>
#endif

/**
 * \enum MutexWaitStatus_t
 * \brief Mutex Stat
 */
typedef enum MutexWaitStatus
{	
	MutexWait_Success, /*!< Mutex Signaled */
	MutexWait_TimeOut, /*!< Mutex waiting time out */
	MutexWait_Failure, /*!< Mutex wait Failure */
	MAX_WAIT_STATUS /*!< Max value of Mutex Wait Status */
	
}MutexWaitStatus_t;

class CMutex
{
public:

	/**********************************************************************************************//**
	 * \fn	CMutex::CMutex(const TC16_t * MutexName = NULL);
	 *
	 * \brief	Initializes a new instance of the CMutex class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	MutexName	(optional) name of the mutex.
	 **************************************************************************************************/

	CMutex(const TC16_t * MutexName = NULL);

	/**********************************************************************************************//**
	 * \fn	CMutex::~CMutex();
	 *
	 * \brief	Finalizes an instance of the CMutex class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	~CMutex();

	/**********************************************************************************************//**
	 * \fn	MutexWaitStatus_t CMutex::Wait(TU32_t MaxWaitingTime = INFINITE);
	 *
	 * \brief	Waits.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	MaxWaitingTime	(optional) time of the maximum waiting.
	 *
	 * \return	.
	 **************************************************************************************************/
	
#if defined (WIN32) || defined (WINCE)
	MutexWaitStatus_t Wait(TU32_t MaxWaitingTime = INFINITE);
#else
	MutexWaitStatus_t Wait();
#endif

	/**********************************************************************************************//**
	 * \fn	bool CMutex::Release();
	 *
	 * \brief	Releases this object.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool Release();

	/**********************************************************************************************//**
	 * \fn	inline bool CMutex::Locked()
	 *
	 * \brief	Lockeds this object.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	inline bool Locked(){return m_MutexLocked;}
private:
	#if defined (WIN32) || defined (WINCE)
	HANDLE m_hMutexHandle;  ///< Handle of the mutex
#else
	pthread_mutex_t m_hMutexHandle;
#endif
	bool m_MutexLocked; ///< true to lock, false to unlock the mutex
};
#endif