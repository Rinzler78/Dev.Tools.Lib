#include "TodoFile.h"
#include "IniFile\IniFile.h"
#include "IniFile\IniSection.h"
#include "IniFile\IniSectionValue.h"
#include "CommonTools\CommonTools.h"
#include "FileInfo\FileInfo.h"
#include "DirectoryInfo\DirectoryInfo.h"
#include "windows.h"
#include "TodoFile_DefaultContent.h"
#include "Log\Log.h"
#include "MyString\MyString.h"

CToDoFile::CToDoFile(TS08_t PathAndFileName, bool DeleteActionAfterExecute, CLog * pLog)
: CConfigFile(PathAndFileName)
, m_pLog(pLog)
, m_pToDoListSection(NULL)
, m_DeleteActionAfterExecute(DeleteActionAfterExecute)
{
	Load();
}
CToDoFile::~CToDoFile()
{
}
void CToDoFile::Load()
{
	bool ValueMissing = false;

	if(!Load_Success())
		CConfigFile::Load();

	if(m_pIniFile->IsLoaded())
	{
		if(m_pLog)
			m_pLog->Log(NOCOMMENTS, "To Do File %s load Success", m_pIniFile->FullName().String());

		if(m_pParameterSection = (*m_pIniFile)[DEFAULT_IPA_CONFIG_FILE_SECTION_NAME_0_])
		{
			if(!(m_pLogpathSectionvalue = (*m_pParameterSection)[DEFAULT_IPA_CONFIG_FILE_SECTION_NAME_0_LogfilePath]))
				ValueMissing = true;
		}
		else
			ValueMissing = true;

		if(!(m_pToDoListSection = (*m_pIniFile)[DEFAULT_IPA_CONFIG_FILE_SECTION_NAME_1_]))
			ValueMissing = true;

		if(ValueMissing)
			m_FileCorrupted = true;
	}
}

bool CToDoFile::CreateDefaultToDoFile()
{
	bool Result = false;

	if(m_pIniFile)
	{
		m_pIniFile->Delete();

		if(m_pLog)
			m_pLog->Log(NOCOMMENTS, "Set Default Content");

		if(Result = m_pIniFile->SetContentInFile(Default_ToDoFile_Content))
		{
			m_LoadedSuccess = false;
			Load();
		}
	}

	return Result;
}
bool CToDoFile::PerformToDoList()
{
	bool Result = false;
	TI32_t Index = 0;
	ToDoActions_t Action = ToDoActions_InitValue;

	if(m_pToDoListSection)
	{
		if(m_pLog)
			m_pLog->Log(NOCOMMENTS, (m_pToDoListSection->ValueList().Count() > 1 ? 
									"%u actions will be perfomed" : "%u action will be perfomed")
									, m_pToDoListSection->ValueList().Count());
		
		while(m_pToDoListSection->ValueList().Count())
		{
			CIniSectionValue * pValue = m_pToDoListSection->ValueList()[0];
			if(pValue)
			{
				if(m_pLog)
					m_pLog->Log(NOCOMMENTS,"Action %s : %s", pValue->ValueName().String(), pValue->Value().String());

				/** Perfom the action */
				if(pValue->ValueName() == ToDoActions_STR_TAB[ToDoActions_Copy])
					Action = ToDoActions_Copy;
				else if(pValue->ValueName() == ToDoActions_STR_TAB[ToDoActions_Move])
					Action = ToDoActions_Move;
				else if(pValue->ValueName() == ToDoActions_STR_TAB[ToDoActions_Delete])
					Action = ToDoActions_Delete;
				else if(pValue->ValueName() == ToDoActions_STR_TAB[ToDoActions_Launch])
					Action = ToDoActions_Launch;

				switch(Action)
				{
					case ToDoActions_Copy:
					case ToDoActions_Move:
						{
							TS08_t Part = NULL;

							Index = pValue->Value().FindFirst(COPY_OR_MOVE_SEPARATOR);
							if(Index > -1)
							{
								Part = new TC08_t[Index];
								memset(Part, 0 ,Index);
								memcpy(Part, pValue->Value().String(), Index);
								
								CFileSystemInfo SourceIO(Part, m_pLog);
#if defined (WINCE)
								CMyString<wchar_t> WSourceIO(SourceIO.FullName());
								DWORD SourceAttribut = GetFileAttributes(CMyString<wchar_t> (SourceIO.FullName()).String());
#elif defined (WIN32)
								DWORD SourceAttribut = GetFileAttributesA(SourceIO.FullName().String());
#endif
								
								if(SourceAttribut != INVALID_FILE_ATTRIBUTES)
								{
									if(SourceAttribut & FILE_ATTRIBUTE_DIRECTORY)
									{
										CDirectoryInfo SourceDirectory((TS08_t)SourceIO.FullName().String(), m_pLog);

										if(SourceDirectory.Exist())
										{
											CDirectoryInfo TargetDirectory(pValue->Value().String() + Index + 2, m_pLog);

											if(!TargetDirectory.Exist())
												TargetDirectory.Create();

											if(TargetDirectory.Exist())
											{
												if(Action == ToDoActions_Copy)
													Result = SourceDirectory.CopyTo(TargetDirectory.FullName().String());
												else
													Result = SourceDirectory.MoveTo(TargetDirectory.FullName().String());
											}
										}
									}
									else
									{
										CFileInfo SourceFile(SourceIO.FullName().String(), m_pLog);
										if(SourceFile.Exist())
										{
											CFileInfo TargetFile(pValue->Value().String() + Index + 1, m_pLog);
											if(TargetFile.Exist())
											{
												if(Action == ToDoActions_Copy)
													Result = SourceFile.CopyTo(TargetFile.FullName().String());
												else
													Result = SourceFile.MoveTo(TargetFile.FullName().String());
											}
										}
									}
								}
							}
						}
						break;
					case ToDoActions_Delete:
						{
							CFileSystemInfo IOToDelete(pValue->Value().String(), m_pLog);
#if defined (WINCE)
							DWORD Attribut = GetFileAttributes(CMyString<wchar_t>(IOToDelete.FullName()).String());
#elif defined (WIN32)
							DWORD Attribut = GetFileAttributesA(IOToDelete.FullName().String());
#endif
								
							if(Attribut != INVALID_FILE_ATTRIBUTES)
							{
								if(Attribut == FILE_ATTRIBUTE_DIRECTORY)
								{
									CDirectoryInfo DirToDelete(IOToDelete.FullName().String(),m_pLog);
									if(DirToDelete.Exist())
										Result = DirToDelete.Delete();
								}
								else
								{
									CFileInfo FileToDelete(IOToDelete.FullName().String(),m_pLog);
									if(FileToDelete.Exist())
										Result = FileToDelete.Delete();
								}
							}
						}
						break;
					case ToDoActions_Launch:
						{
							/** launch */
							CFileInfo AppToLaunch(pValue->Value().String(), m_pLog);
							if(AppToLaunch.Exist())
							{
								Result = (AppToLaunch.Execute() == ING_STATUS_SUCCESS);
							}
						}
						break;
					default:
						if(m_pLog)
							m_pLog->Log(ALERT,"Unrecognized Command : %s", pValue->ValueName().String());
						break;
				}
			}

			if(Result)
			{
				if(m_pLog)
					m_pLog->Log(NOCOMMENTS,"To Do Action Success");
			}
			else
			{
				if(m_pLog)
					m_pLog->Log(ALERT,"To Do Action Failure");
			}
			
			
			m_pToDoListSection->ValueList().DeleteFirst();

			/** Delete Value */
			MACRO_DELET(pValue)

			/** Update Ini File */
			if(m_DeleteActionAfterExecute)
			{
				/** Remove Action From list */
				if(m_pLog)
					m_pLog->Log(NOCOMMENTS,"Delete Action from list");

				m_pIniFile->UpdateFile();
			}
		}
	}

	return Result;
}
bool CToDoFile::AddActionOnToDoList(ToDoActions_t Action, TS08_t ActionExplain)
{
	bool Result = false;
	
	if(m_pToDoListSection)
	{
		if(m_pLog)
			m_pLog->Log(NOCOMMENTS, "Add Action : %s : %s", ToDoActions_STR_TAB[Action], ActionExplain);

		Result = m_pToDoListSection->ValueList().Add(new CIniSectionValue((TS08_t)ToDoActions_STR_TAB[Action], ActionExplain));
	}
	return Result;
}