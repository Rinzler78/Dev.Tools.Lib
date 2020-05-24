#ifndef CTHREADMANAGER_H
#define CTHREADMANAGER_H
#include "Common\MyTypes.h"
#include "LinkedList\LinkedList.h"

//class CLog;
class CThread;

class CThreadManager
{
public : 
	//static CLinkedList<CThread> GetThreadList();

	/**********************************************************************************************//**
	 * \fn	static bool CThreadManager::IsThreadAlive(TU32_t ThreadID);
	 *
	 * \brief	Query if 'ThreadID' is thread alive.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	ThreadID	Identifier for the thread.
	 *
	 * \return	true if thread alive, false if not.
	 **************************************************************************************************/

	static bool IsThreadAlive(TU32_t ThreadID);
};
#endif