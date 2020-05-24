#ifndef CFILESYSTEMINFO_CPP
#define CFILESYSTEMINFO_CPP
#include "FileSystemInfo\FileSystemInfo.h"

#if defined (WIN32) || defined (WINCE)
#include "windows.h"
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cwchar> 
#endif

#include "Log\Log.h"
#include "CommonTools\CommonTools.h"
#include "MyString\MyString.h"
#include "DirectoryInfo\DirectoryInfo.h"

CFileSystemInfo::CFileSystemInfo(const TS08_t pPathAndFileName, CLog *pLog)
:m_pName(NULL)
, m_pFullName(NULL)
, m_pExtension(NULL)
, m_Size(0)
, m_pParentDirectory(NULL)
, m_pLog(pLog)
, m_Exist(false)
, m_CRC16(0)
{
	SetFileSystemInfo(pPathAndFileName);
}
CFileSystemInfo::CFileSystemInfo(const CMyString<char> & PathAndFileName, CLog *pLog)
:m_pName(NULL)
, m_pFullName(NULL)
, m_pExtension(NULL)
, m_Size(0)
, m_pParentDirectory(NULL)
, m_pLog(pLog)
, m_Exist(false)
, m_CRC16(0)
{
	SetFileSystemInfo(PathAndFileName.String());
}
CFileSystemInfo::~CFileSystemInfo()
{
	MACRO_DELET(m_pParentDirectory)
	MACRO_DELET(m_pName)
	MACRO_DELET(m_pFullName)
	MACRO_DELET(m_pExtension)
}
void CFileSystemInfo::Refresh() 
{

}
CMyString<char> CFileSystemInfo::GetCurrent_Directory()
{
	CMyString<char> & Temp = GetCurrent_AppFullPath();
	return CMyString<char>(Temp.FindLast('\\'), Temp.String());
}
CMyString<char> CFileSystemInfo::ResolvePath(const TS08_t pPathAndFileName)
{
	//CMyString<char> * pResult = NULL;
	CMyString<char> Temp;
	TU32_t PathLength = 0;
	TU32_t NbParentsDirectory = 0;
	TU32_t Bufferpos = 0;
	TU32_t ParentsDirectoryIndex = 0;

	if(pPathAndFileName)
	{
		CMyString<char> Path(pPathAndFileName);
		Path.SupressBefore(' ');
		Path.SupressAfter(' ');
		Path.Replace('/', '\\');

		/** Get Directory */
		if((Path.Length() >= 2) && (Path.String()[0] == '.') && (Path.String()[1] == '\\'))
		{
			/** Get Current Path */
			Temp = GetCurrent_Directory();
			
			if(Temp.Length() > 2)
				Temp.AddString("\\%s", &Path.String()[2]);
		}
		else 
		{
			if(	(Path.Length() >= 3)
				&& (Path[Bufferpos] == '.') 
				&& (Path[Bufferpos + 1] == '.') 
				&& (Path[Bufferpos + 2] == '\\'))
			{
				NbParentsDirectory++;
				Bufferpos += 3;
			
				while(	Path[Bufferpos] == '.' && Path[Bufferpos + 1] == '.' 
					&& Path[Bufferpos + 2] == '\\')
				{
					NbParentsDirectory++;
					Bufferpos += 3;
				}
				
				Path.SupressBefore("..\\");
				/** Get Current Directory */
				CMyString<char> CurrentDirectory = GetCurrent_Directory();

				/** Find Parent Directory in CurrentDirectory */
				for(ParentsDirectoryIndex = CurrentDirectory.Length() ; (ParentsDirectoryIndex > 0) ;--ParentsDirectoryIndex)
				{
					if((CurrentDirectory[ParentsDirectoryIndex] == '\\'))
					{
						NbParentsDirectory--;
						if(NbParentsDirectory == 0)
							break;
					}
				}

				Temp.AddString(ParentsDirectoryIndex, CurrentDirectory.String());
				Temp.AddString("\\%s",Path.String());
			}
			else
				Temp += Path;
		}
	}

	return Temp;
}

CMyString<char> CFileSystemInfo::GetCurrent_AppName()
{
	CMyString<char> & Temp = GetCurrent_AppFullPath();
	return CMyString<char>(Temp.String() + Temp.FindLast('\\') + 1);
}
CMyString<char> CFileSystemInfo::GetCurrent_AppFullPath()
{
	CMyString<char> Result;
	TC16_t ExecutablePathAndFileName [FULLNAME_MAX_NB_LETTER + 50] = {0};
	TU32_t ExecutablePathAndFileNameSize = 0;
	TU32_t LastSlashIndex = 0;
	
	/** Get Executable Path And File name */
	GetModuleFileName(NULL,ExecutablePathAndFileName,FULLNAME_MAX_NB_LETTER + 50);
	Result = ExecutablePathAndFileName;

	return Result;
}
void CFileSystemInfo::SetFileSystemInfo(const TS08_t pPathAndFileName)
{
	if(pPathAndFileName)
	{
		/** Full Name */
		m_pFullName = new CMyString<char>(ResolvePath(pPathAndFileName));
		m_pFullName->SupressBefore(' ');
		m_pFullName->SupressAfter(' ');

		/** Set Name */
		TI32_t lastIndex = 0;
			
		lastIndex = m_pFullName->FindLast('\\');

		if(lastIndex > -1)
		{
			/** Set File Name */
			m_pName = new CMyString<char>(m_pFullName->String() + lastIndex + 1);

			/** Set File Extension */
			TI32_t DotIndex = m_pName->FindLast('.');
			if(DotIndex > -1)
				m_pExtension = new CMyString<char>(m_pName->String() + DotIndex + 1);
			else
				m_pExtension = new CMyString<char>();

#if defined (UNDER_CE)
			if(lastIndex > 0) /** On ce Main directory is \ */
#else
			if(lastIndex > 2) 
#endif
			{
				/** Create Parent Directory */
				m_pParentDirectory = new CDirectoryInfo(CMyString<char>(lastIndex, m_pFullName->String()), m_pLog);
			}
		}
#if defined (WINCE)
		DWORD Attributs = GetFileAttributes(CMyString<wchar_t>(m_pFullName).String());
		m_Exist = (Attributs != INVALID_FILE_ATTRIBUTES);
#elif defined (WIN32)
		DWORD Attributs = GetFileAttributesA(m_pFullName->String());
		m_Exist = (Attributs != INVALID_FILE_ATTRIBUTES);
#else
		struct stat st;
		m_Exist = (stat(m_FullName,&st) == 0);
#endif
	}
}
#endif