// CleanFileSystem.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "Common\MyTypes.h"
#include "LinkedList\LinkedList.h"
#include "DirectoryInfo\DirectoryInfo.h"
#include "Log\Log.h"
#include "MyString\MyString.h"
#include "FileInfo\FileInfo.h"
#define AUTOLOG_ARG L"-al"
#define AUTOLOG_ARG_verbose L"-alv"
#define SPECIFIED_LOG_ARG L"-sl"
#define SPECIFIED_LOG_ARG_verbose L"-slv"

bool IsMustBeSkip(const CMyString<char> & CurrentDirectory, CLinkedList<CMyString<char> *> &SkipList);

int _tmain(int argc, _TCHAR* argv[])
{
	ING_STATUS_t Return = ING_STATUS_SUCCESS;
	CLog Trace;
	CLinkedList<CMyString<char> *> DirectoryList;

	/** Retrieve directories to skip */
	for(TU08_t i = 1 ; i < argc ; ++i)
	{
		if(!wcscmp(AUTOLOG_ARG,argv[i]))
		{
			/** Active Log With Default Log Name */
			Trace.SetPathAndFileName("%w.txt", argv[0]);
			Trace.Active_Debug(true);
		}
		else if(!wcscmp(SPECIFIED_LOG_ARG,argv[i]))
		{
			++i;
			/** Active Log With Default Log Name */
			Trace.SetPathAndFileName("%w", argv[i]);
			Trace.Active_Debug();
		}
		if(!wcscmp(AUTOLOG_ARG_verbose,argv[i]))
		{
			/** Active Log With Default Log Name */
			Trace.SetPathAndFileName("%w.txt", argv[0]);
			Trace.Active_Debug(true, LOG_OUT_FILE | LOG_OUT_CONSOLE);
		}
		else if(!wcscmp(SPECIFIED_LOG_ARG_verbose,argv[i]))
		{
			++i;
			/** Active Log With Default Log Name */
			Trace.SetPathAndFileName("%w", argv[i]);
			Trace.Active_Debug(false, LOG_OUT_FILE | LOG_OUT_CONSOLE);
		}
		else
		{
			if(wcslen(argv[i]))
				DirectoryList.Add(new CMyString<char>(argv[i]));
		}
	}
	
	if(argc == 1)
		Trace.Active_Debug(false,LOG_OUT_CONSOLE,false,false);

	Trace.Log(NOCOMMENTS, "Start");
	CDirectoryInfo MainDirectory("/", &Trace);
	MainDirectory.Refresh();

	for(TU32_t i = 0 ; i < MainDirectory.GetDirectories().Count() ; ++i)
	{
		CDirectoryInfo * pCurrentDir = NULL;
		pCurrentDir = MainDirectory.GetDirectories()[i];
		if(pCurrentDir)
		{
			if(IsMustBeSkip(pCurrentDir->FullName(), DirectoryList))
			{
				Trace.Log(NOCOMMENTS,"Skip %s", pCurrentDir->FullName().String());
				continue;
			}
			
			/** Delete Directory and all of childs */
			pCurrentDir->Delete();
		}
		else
			Trace.Log(ALERT,"Directory %u pointer empty", i);
	}

	for(TU32_t i = 0 ; i < MainDirectory.GetFiles().Count() ; ++i)
	{
		CFileInfo * pCurrentFile = NULL;
		pCurrentFile = MainDirectory.GetFiles()[i];
		if(pCurrentFile)
			pCurrentFile->Delete();
		else
			Trace.Log(ALERT,"File %u pointer empty", i);
	}
	Trace.Log(NOCOMMENTS, "End");

	return Return;
}
bool IsMustBeSkip(const CMyString<char> & CurrentDirectory, CLinkedList<CMyString<char> *> &SkipList)
{
	bool Result = false;
	
	for(TU08_t i = 0 ; i < SkipList.Count() ; ++i)
	{
		if((*SkipList[i]) == CurrentDirectory)
		{
			Result = true;
			break;
		}
	}

	return Result;
}
