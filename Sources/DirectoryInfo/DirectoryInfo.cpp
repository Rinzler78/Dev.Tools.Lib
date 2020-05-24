#include "DirectoryInfo\DirectoryInfo.h"
#include "CommonTools\CommonTools.h"
#include <iostream>

#if defined (WIN32) || defined (WINCE)
#include "windows.h"
#endif

#include "Log\Log.h"
#include <string>
#include "CrcTool\CrcTool.h"
#include "LinkedList\LinkedList.h"
#include "FileInfo\FileInfo.h"
#include "MyString\MyString.h"

#if ! (defined (WIN32) || defined (WINCE))
#include <boost/filesystem.hpp>
using namespace boost::filesystem;
#endif

CDirectoryInfo::CDirectoryInfo(const TS08_t  pPathDirectory, CLog *pLog )
:CFileSystemInfo(pPathDirectory, pLog)
, m_pChildDirectoryList(new CLinkedList<CDirectoryInfo *>())
, m_pChildFilesList(new CLinkedList<CFileInfo *>())
{
}
CDirectoryInfo::CDirectoryInfo(const CMyString<char> & PathDirectory, CLog *pLog)
:CFileSystemInfo(PathDirectory, pLog)
, m_pChildDirectoryList(new CLinkedList<CDirectoryInfo *>())
, m_pChildFilesList(new CLinkedList<CFileInfo *>())
{
}
CDirectoryInfo::~CDirectoryInfo()
{
	ClearDirectorylist();
	MACRO_DELET(m_pChildDirectoryList)
	ClearFilelist();
	MACRO_DELET(m_pChildFilesList)
}

bool CDirectoryInfo::Create()
{
	bool Result = false;
	DWORD lasterr = 0;

#if defined (WIN32) || defined (WINCE)
	CMyString<wchar_t> WFullName(FullName());
	Result = (CreateDirectory(WFullName.String(),NULL) == TRUE);
#else
	path DirectoryPath(m_FullName);
	Result = create_directories(DirectoryPath);
#endif
	if(Result)
	{
		if(m_pLog)m_pLog->Log(NOCOMMENTS, "Create Directory %s Success", FullName().String());
	}
	else
	{
#if defined (WIN32) || defined (WINCE)
		lasterr = GetLastError();
#endif
		if(m_pLog)m_pLog->Log(NOCOMMENTS, "Create Directory %s Failure (code %u)", FullName().String(), lasterr);
	}

	m_Exist = Result;

	return Result;
}
bool CDirectoryInfo::Delete()
{
	bool Result = false;
	Refresh();/** Refresh Directory and Files List */
	
	/** Remove Childs Directories */
	for(TU32_t i = 0 ; i < GetDirectories().Count() ; ++i)
	{
		CDirectoryInfo * pCurrentDirectory = NULL;
		pCurrentDirectory = GetDirectories()[i];

		if(pCurrentDirectory)
		{
			if(pCurrentDirectory->Delete())
			{
				GetDirectories().DeleteAt(i);
				MACRO_DELET(pCurrentDirectory);
				--i;
			}
		}
	}

	/** Remove Childs Files */
	for(TU32_t i = 0 ; i < GetFiles().Count() ; ++i)
	{
		CFileInfo * pCurrentFile = NULL;
		pCurrentFile = GetFiles()[i];

		if(pCurrentFile)
		{
			if(pCurrentFile->Delete())
			{
				GetFiles().DeleteAt(i);
				MACRO_DELET(pCurrentFile);
				--i;
			}
		}
	}
	
#if defined (WIN32) || defined (WINCE)
	CMyString<wchar_t> WString(FullName());
	Result = (RemoveDirectory(WString.String()) == TRUE);
#else
	path DirectoryPath(m_FullName);
	Result = remove(DirectoryPath);
#endif
	if(Result)
	{
		if(m_pLog)m_pLog->Log(NOCOMMENTS, "Delete Directory %s Success", FullName().String());
	}
	else
		if(m_pLog)m_pLog->Log(NOCOMMENTS, "Delete Directory %s Failure", FullName().String());

	m_Exist = !Result;

	return Result;
}
void CDirectoryInfo::Refresh()
{
#if defined (WIN32) || defined (WINCE)
	DWORD dwAttr = FILE_ATTRIBUTE_DIRECTORY;
	WIN32_FIND_DATA fd;
#else
#endif
	HANDLE hFind = NULL;
	TU32_t Size = 0;
	TC16_t DirectoryFoundedPath[FULLNAME_MAX_NB_LETTER] = {0};
	
	ClearDirectorylist();
	ClearFilelist();

	if(m_pLog)
		m_pLog->Log(NOCOMMENTS, "Scanning %s", FullName().String());
	
	CMyString<wchar_t> SearchPath(FullName());
	
	if((SearchPath[SearchPath.Length() - 1] != L'\\'))
		SearchPath += L"\\*.*";
	else
		SearchPath += L"*.*";

	hFind = FindFirstFile( SearchPath.String(), &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		CMyString<wchar_t> WFullName(FullName());
		do
		{
			if((wcscmp(fd.cFileName, L".") != 0) && (wcscmp(fd.cFileName, L"..") != 0))
			{
				wsprintf(	DirectoryFoundedPath
							, (WFullName.IsEndBy('\\') ? L"%s%s":L"%s\\%s")
							, WFullName.String()
							, fd.cFileName);

				if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					GetDirectories().Add(new CDirectoryInfo(DirectoryFoundedPath,m_pLog));
				else
					GetFiles().Add(new CFileInfo(DirectoryFoundedPath,m_pLog));

				MACRO_MEMSET(DirectoryFoundedPath, sizeof(DirectoryFoundedPath))
			}
		}while(FindNextFile(hFind,&fd));

		FindClose(hFind);
	}


	if(m_pLog)m_pLog->Log(	NOCOMMENTS
							, (GetDirectories().Count() > 1 ? "%i child folders found" : "%i child folder found")
							, GetDirectories().Count());

	if(m_pLog)m_pLog->Log(	NOCOMMENTS
							, (GetFiles().Count() > 1 ? "%i child files found" : "%i child file found")
							, GetFiles().Count());

}
bool CDirectoryInfo::CopyTo(const TS08_t  pPath, bool EraseIfExist)
{
	bool Result = false;
	
	TC08_t NewDest_STR [FULLNAME_MAX_NB_LETTER] = {0};
	
	if(pPath)
	{
		Result = true;

		Refresh();/** Refresh Directory and Files List */
		
		CDirectoryInfo NewDestFolder(pPath,m_pLog);

		if(!NewDestFolder.Exist())
			NewDestFolder.Create();

		/** Copy Childs Directories */
		for(TU32_t i = 0 ; i < GetDirectories().Count() ; ++i)
		{
			CDirectoryInfo * pCurrentDirectory = NULL;
			pCurrentDirectory = GetDirectories()[i];

			if(pCurrentDirectory)
			{
				if(NewDestFolder.FullName().IsEndBy('\\'))
					sprintf(NewDest_STR, "%s%s", NewDestFolder.FullName().String(), pCurrentDirectory->Name().String());
				else
					sprintf(NewDest_STR, "%s\\%s",NewDestFolder.FullName().String(), pCurrentDirectory->Name().String());

				Result = pCurrentDirectory->CopyTo(NewDest_STR);
				MACRO_MEMSET(NewDest_STR, sizeof(NewDest_STR))
			}
		}

		/** Copy Childs Files */
		if(Result)
		{
			for(TU32_t i = 0 ; i < GetFiles().Count() ; ++i)
			{
				CFileInfo * pCurrentFile = NULL;
				pCurrentFile = GetFiles()[i];

				if(pCurrentFile)
				{
					if(NewDestFolder.FullName().IsEndBy('\\'))
						sprintf(NewDest_STR, "%s%s", NewDestFolder.FullName().String(), pCurrentFile->Name().String());
					else
						sprintf(NewDest_STR, "%s\\%s",NewDestFolder.FullName().String(), pCurrentFile->Name().String());

					Result = pCurrentFile->CopyTo(NewDest_STR);

					MACRO_MEMSET(NewDest_STR, sizeof(NewDest_STR))
				}
			}
		}

		if(!Result && m_pLog)
			m_pLog->Log(ALERT,"Copy %s to %s failure", FullName().String(), NewDestFolder.FullName().String());
	}
	
	
	return Result;
}
bool CDirectoryInfo::MoveTo(const TS08_t  pPath, bool EraseIfExist)
{
	bool Result = false;
		
	if(pPath)
	{

	}

	return Result;
}
bool CDirectoryInfo::RetrieveCRC16()
{
	bool Result = false;
	
	m_CRC16 = CCrcTool::GetDirectoryCRC16(this);

	Result = (m_CRC16 != 0);

	return Result;
}
void CDirectoryInfo::ClearDirectorylist()
{
	/** Child Directroy List */
	if(GetDirectories().Count() > 0)
		if(m_pLog)
			m_pLog->Log(NOCOMMENTS, "Clear Child File list of %s", FullName().String());

	while(GetDirectories().Count())
	{
		CDirectoryInfo *pDir = NULL;
		pDir = GetDirectories()[0];
		MACRO_DELET(pDir);
		
		GetDirectories().DeleteAt(0);
	}
}
void CDirectoryInfo::ClearFilelist()
{
	/** Child Directroy List */
	if(GetFiles().Count() > 0)
		if(m_pLog)
			m_pLog->Log(NOCOMMENTS, "Clear Child Directory list of %s", FullName().String());

	while(GetFiles().Count())
	{
		CFileInfo *pDir = NULL;
		pDir = GetFiles()[0];
		MACRO_DELET(pDir);
		
		GetFiles().DeleteAt(0);
	}
}