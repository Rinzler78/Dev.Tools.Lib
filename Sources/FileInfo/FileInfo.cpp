#include "FileInfo\FileInfo.h"
#include "DirectoryInfo\DirectoryInfo.h"
#include "Mutex\Mutex.h"
#include "CommonTools\CommonTools.h"
#include "CrcTool\CrcTool.h"
#include <iostream>
#include <fstream>
#include "Log\Log.h"
#include "MyString\MyString.h"
#if ! (defined (WIN32) || defined (WINCE))
#include <boost/filesystem.hpp>
using namespace boost::filesystem;
#endif

using namespace std;
static CMutex FileAccessMutex(L"File Access Mutex");
const TC08_t TabFileOpen_Attribute[MAX_OPEN_ATTRIBUT][MAX_OPEN_ATTRIBUT] \
= {	"", "r", "w", "a", "r+", "w+", "a+", "rb", "wb", "ab", "r+b", "w+b", "a+b"};

CFileInfo::CFileInfo(const TS08_t  pPathAndFileName, CLog *pLog) 
:CFileSystemInfo(pPathAndFileName, pLog)
, m_FileOpened (false)
, m_pFile (NULL)
, m_FileCurrentOpeningAttribut(FileOpen_Initvalue)
, m_pFileContent(NULL)
, m_BlockAutoClose(false)
{
	Refresh();
}
CFileInfo::CFileInfo(const CMyString<char> & PathAndFileName, CLog *pLog)
:CFileSystemInfo(PathAndFileName, pLog)
, m_FileOpened (false)
, m_pFile (NULL)
, m_FileCurrentOpeningAttribut(FileOpen_Initvalue)
, m_pFileContent(NULL)
, m_BlockAutoClose(false)
{
	Refresh();
}
CFileInfo::~CFileInfo()
{
	MACRO_DELET_TAB(m_pFileContent)
}
void CFileInfo::Refresh()
{
	/** Refresh File Existance */
	if(m_Exist)
	{
		Open(OPEN_IN_READ_ATTRIBUTE);
		m_Size = GetFileSize(false);
		Close();
	}
}
bool CFileInfo::Open(FileOpen_Attribute_t OpenAtt)
{
	bool Result = false;
	
	if(m_FileOpened && (m_FileCurrentOpeningAttribut != OpenAtt))
		Close();

	if(!m_FileOpened)
	{
		FileAccessMutex.Wait();
		m_pFile = fopen((const TS08_t)FullName().String()
			, TabFileOpen_Attribute[OpenAtt]);
		
		if(m_pFile)
			m_FileCurrentOpeningAttribut = OpenAtt;
		else
			FileAccessMutex.Release();
		Result = m_Exist = m_FileOpened = (m_pFile != NULL);
	}
	return Result;
}
bool CFileInfo::WriteLine(const TS08_t pCharStringToWrite,...)
{
	bool Result = false;
	va_list vl;
	
	if(pCharStringToWrite)
	{
		
		/** open File it it isn't */
		if(!m_FileOpened)
			Open(OPEN_BOTH_READ_APPEND_ATTRIBUTE);

		/** Open File */
		if(m_FileOpened)
		{
			va_start(vl, pCharStringToWrite);
			CMyString<char> StringToWrite = CMyString<char>::GetVarList(vl, pCharStringToWrite);
			va_end(vl);

			/** Write To File */
			Result = (fprintf(m_pFile,"%s\n", StringToWrite.String()) > 0);

			/** Close File */
			if(!m_BlockAutoClose)
				Result = Close();
		}
	}
	return Result;
}
bool CFileInfo::CopyTo(const TS08_t  pPathAndFileName, bool EraseIfExist)
{
	bool Result = false;

	if(pPathAndFileName)
	{
		if(m_FileOpened)
			Close();
		
		if(Exist())
		{
			CFileInfo Destination(pPathAndFileName);
			
			if(EraseIfExist && Destination.Exist())
				Destination.Delete();

			CMyString<wchar_t> Src(FullName());
			CMyString<wchar_t> Trg(Destination.FullName());

#if defined (WIN32) || defined (WINCE)
			Result = (CopyFile(Src.String(), Trg.String(), EraseIfExist) == TRUE);
#else
			boost::system::error_code CopyError;
			path Dest(m_FullName);
			path Source(pPathAndFileName);
			copy_file(Dest, Source, (EraseIfExist ? boost::filesystem3::copy_option::overwrite_if_exists : boost::filesystem3::copy_option::fail_if_exists), CopyError);
			
			Result = (CopyError.value() == 0);
#endif
			if(Result)
			{
				if(m_pLog)m_pLog->Log(NOCOMMENTS, "Copy %s to %s Success", FullName().String(), Destination.FullName().String());
			}
			else
			{
				if(m_pLog)m_pLog->Log(ALERT
#if defined (WIN32) || defined (WINCE)
									  , "Copy %s to %s Failure (code %u)"
#else
									  , "Copy %s to %s Failure"
#endif
									  , FullName().String()
									  , Destination.FullName().String()
									  #if defined (WIN32) || defined (WINCE)
									  , GetLastError()
#endif
									  );
				
#if defined (WIN32) || defined (WINCE)
				DWORD ErrorCode = GetLastError();

				if(SetFileAttributes(Trg.String(),FILE_ATTRIBUTE_NORMAL))
					Result = (CopyFile(Src.String(), Trg.String(), EraseIfExist) == TRUE);
#endif
			}
		}
		else
			if(m_pLog)m_pLog->Log(ALERT, "Source file %s not exist", FullName().String());
	}
	return Result;
}
bool CFileInfo::MoveTo(const TS08_t  pPathAndFileName, bool EraseIfExist)
{
	bool Result = false;

	if(pPathAndFileName)
	{
		if(m_FileOpened)
			Close();

		if(Exist())
		{
			CFileInfo Destination(pPathAndFileName);

			if(EraseIfExist && Destination.Exist())
				Destination.Delete();
			
			if(CopyTo(pPathAndFileName))
			{
				CMyString<wchar_t> WFullName(FullName());
#if defined (WIN32) || defined (WINCE)
				Result = (DeleteFile(WFullName.String()) == TRUE);
#else
				remove(m_FullName);
#endif
			}
		}

	}
	return Result;
}
bool CFileInfo::Write(const TS08_t pCharStringToWrite, bool Auto_Close)
{
	bool Result = false;
	
	if(pCharStringToWrite)
	{
		/** open File it it isn't */
		if(!m_FileOpened)
			Open(OPEN_IN_WRITE_ATTRIBUTE);

		/** Write To File */
		if(m_FileOpened)
			Result = (fprintf(m_pFile,"%s",pCharStringToWrite) > 0);
		else
		{
			if(m_pLog)
				m_pLog->Log(ALERT,"Cannot write in %s -> create a new file", FullName().String());

			if(Create())
				Result = Write(pCharStringToWrite,Auto_Close);
		}
		
		/** refresh file info */
		Refresh();
		/** Close File */
		if(Auto_Close && m_FileOpened)
			Result = Close();
	}
	return Result;
}
TU64_t CFileInfo::ReadContent(TS08_t Out_Buffer, TU64_t MaxSize, TU32_t StartIndex)
{
	TU64_t Result = 0;
	
	if(Out_Buffer && MaxSize > 0)
	{
		Open(OPEN_IN_READ_ATTRIBUTE_AS_BINARY);

		if(m_FileOpened)
		{
			if(StartIndex > 0 && StartIndex < m_Size)
			{
				fpos_t Position = StartIndex;
				fsetpos(m_pFile, &Position);
			}

			Result = (TU64_t)fread (Out_Buffer,1,MaxSize,m_pFile);
			Close();
		}
	}
	return Result;
}
bool CFileInfo::Delete()
{
	bool Result = false;
	if(m_Exist)
	{
		CMyString<wchar_t> WFullName(FullName());
#if defined (WIN32) || defined (WINCE)
		Result = (DeleteFile(WFullName.String()) == TRUE);
#else
		Result = remove(m_FullName);
#endif
		if(Result)
		{
			if(m_pLog)m_pLog->Log(NOCOMMENTS, "Delete File %s Success", FullName().String());
		}
		else
		{
#if defined (WIN32) || defined (WINCE)
			/** Change file attribut if read Only to delete it*/
			DWORD FileAttribut = GetFileAttributes(WFullName.String());
			if(FileAttribut & FILE_ATTRIBUTE_READONLY)
			{
				if(SetFileAttributes(WFullName.String(), FILE_ATTRIBUTE_NORMAL))
				{
					Result = (DeleteFile(WFullName.String()) == TRUE);
				}
				else
					if(m_pLog)m_pLog->Log(NOCOMMENTS, "Set File Attribut %s Failure", FullName().String());
			}
#endif

			if(!Result)
				if(m_pLog)m_pLog->Log(NOCOMMENTS, "Delete File %s Failure", FullName().String());
		}

		Refresh();
	}
	return Result;
}
bool CFileInfo::Create()
{
	bool Result = false;
	
	if((Result = Open(OPEN_IN_WRITE_ATTRIBUTE)))
	{
	   if(m_pLog)m_pLog->Log(NOCOMMENTS,"Creation of %s success", FullName().String());
	}
	else
	   if(m_pLog)m_pLog->Log(ALERT,"Creation of %s failure", FullName().String());

	return Result;
}
bool CFileInfo::Close()
{
	bool Result = false;

	if(m_FileOpened)
	{
		m_FileOpened = !(fclose(m_pFile) == 0);
		if(!m_FileOpened)
		{
			FileAccessMutex.Release();
			m_pFile = NULL;
			m_FileCurrentOpeningAttribut = FileOpen_Initvalue;
		}
		Result = !m_FileOpened;
	}

	return Result;
}
ING_STATUS_t CFileInfo::Execute(const TS08_t  pArguments, bool WaitUntilExit)
{
	TU32_t Result = 0;
	CMyString<wchar_t> Arguments(pArguments);
	
	if(m_pLog)
		m_pLog->Log(NOCOMMENTS
						, (WaitUntilExit ? "Launch %s Args(%s) Wait until Exit" : "Launch %s Args(%s)")
						, FullName().String()
						, pArguments);
	if(m_Exist)
	{
#if defined (_DEBUG)
		Sleep(1000);
#endif
		CMyString<wchar_t> WFullName(FullName());

		if(!(CCommonTools::Launchprogram(WFullName.String(), (TU32_t)Result, Arguments.String(), WaitUntilExit)))
		{
			if(m_pLog)
				m_pLog->Log(ALERT, "Launch %s Failure (Return code %u -> %s)", FullName().String(), Result, ING_STATUS_STR_TAB[Result]);
		}
		else
		{
			if(m_pLog)
				m_pLog->Log(NOCOMMENTS, "Launch %s Success  (Return code %u -> %s)", FullName().String(), Result, (!WaitUntilExit ? ING_STATUS_STR_TAB[ING_STATUS_SUCCESS] : ING_STATUS_STR_TAB[Result]));
			if(!WaitUntilExit)
				Result = (TU32_t)ING_STATUS_SUCCESS;
		}
	}
	else
		if(m_pLog)
			m_pLog->Log(ALERT, "%s does not exist -> Cannot Execute", FullName().String());

	return (ING_STATUS_t)Result;
}
TU64_t CFileInfo::GetFileSize(bool CloseFileAfter)
{
	TU64_t Result = 0;
	
	if(m_Exist)
	{
		if(!m_FileOpened)
			Open(OPEN_IN_READ_ATTRIBUTE_AS_BINARY);
		if(m_FileOpened)
		{
			fseek (m_pFile , 0 , SEEK_END);
			Result = ftell (m_pFile);
#if defined (UNDER_CE)
			fseek (m_pFile , 0 , SEEK_SET);
#else
			rewind (m_pFile);
#endif
			
			if(CloseFileAfter)
			{
				/** Close File */
				Close();
			}
		}
	}

	return Result;
}

bool CFileInfo::RetrieveCRC16()
{
	bool Result = false;
	
	m_CRC16 = CCrcTool::GetCRC16(FullName().String());

	Result = (m_CRC16 != 0);

	return Result;
}
bool CFileInfo::LoadContent()
{
	bool Result = false;
	TU32_t SizeReaded = 0;
	
	if(Exist())
	{
		if(m_pLog)
			m_pLog->Log(NOCOMMENTS, "Load File Content");

		if(!m_FileOpened)
			Open(OPEN_IN_READ_ATTRIBUTE);

		if(m_FileOpened)
		{
			MACRO_DELET_TAB(m_pFileContent);

			m_pFileContent = new TC08_t[Size() + 1];

			if(m_pFileContent)
			{
				MACRO_MEMSET(m_pFileContent, Size() + 1)
				SizeReaded = ReadContent(m_pFileContent, Size());
				Result = (SizeReaded == Size());
			}
			Close();
		}
	}

	return Result;
}