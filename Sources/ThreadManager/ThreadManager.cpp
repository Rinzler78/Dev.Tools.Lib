#include "ThreadManager\ThreadManager.h"
#include <windows.h>
#include <tlhelp32.h>

bool CThreadManager::IsThreadAlive(TU32_t ThreadID)
{
	bool Result = false;
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE; 
	THREADENTRY32 te32 = {0}; 
 
	// Take a snapshot of all running threads  
	hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 ); 
	if( hThreadSnap != INVALID_HANDLE_VALUE )
	{
		// Fill in the size of the structure before using it. 
		te32.dwSize = sizeof(THREADENTRY32 ); 

		// Retrieve information about the first thread,
		// and exit if unsuccessful
		if( Thread32First( hThreadSnap, &te32 ) ) 
		{
			do 
			{ 
				if( te32.th32ThreadID == ThreadID )
				{
					Result = true;
					break;
				}
			} while( Thread32Next(hThreadSnap, &te32 ) );
		}
		//  Don't forget to clean up the snapshot object.
		CloseHandle( hThreadSnap );
	}
	
	return Result;
}