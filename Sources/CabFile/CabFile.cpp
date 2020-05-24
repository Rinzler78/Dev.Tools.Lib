#include "CabFile.h"
#include "CommonTools\CommonTools.h"
#include "windows.h"
#include "Registry\Regkey.h"
#include "MyString\MyString.h"
#include "Log\Log.h"
#define ARGUMENTS_LENGTH					200
#define INSTCAB_PROCESS_NAME                L"wceload.exe"
#define UNINSTCAB_PROCESS_NAME              L"unload.exe"
#define MAX_ARGUMENT_NBLETTER				MAX_PATH
#define INSTALL_CAB_ARGUMENTS_DELETE		"/delete"
#define INSTALL_CAB_ARGUMENTS_NOPROMPT		"/noui"
#define INSTALL_CAB_ARGUMENTS_NOASKDEST		"/noaskdest"
#define INSTALL_CAB_ARGUMENTS_ASKDEST		"/askdest"
#define INSTALL_CAB_ARGUMENTS_NOUNINSTALL	"/nouninstall"
#define APPS_INSTALLED_REG_LOCATION          "SOFTWARE\\Apps"
#define CAB_LOCATION_KEY_NAME				L"CabFile"
#define CAB_INSTALLED_INFO_KEY_NAME			L"Instl"

CCabFile::CCabFile(const TS08_t  pPathAndFileName, CLog * pLog)
:CFileInfo(pPathAndFileName)
, m_Installed(false)
, m_pLog(pLog)
{
	MACRO_MEMSET(m_CabName, sizeof(m_CabName));
}
CCabFile::CCabFile(const TS16_t  pPathAndFileName, CLog * pLog)
:CFileInfo(pPathAndFileName)
, m_Installed(false)
, m_pLog(pLog)
{
	MACRO_MEMSET(m_CabName, sizeof(m_CabName));
}
CCabFile::~CCabFile()
{
}
void CCabFile::Refresh()
{
	m_Installed = false;
	
	CRegKey CabAppsKey(HKEY_LOCAL_MACHINE,APPS_INSTALLED_REG_LOCATION);

	if(m_pLog)
		m_pLog->Log(NOCOMMENTS, "Refresh cab informations");

	CabAppsKey.Refresh();
	for(TU32_t i = 0 ; (i < CabAppsKey.GetKeys()->Count()) && !m_Installed ; ++i)
	{
		CRegKey *pKey = NULL;
		pKey = (*CabAppsKey.GetKeys())[i];

		if(pKey)
		{
			pKey->Refresh();
			bool CabLocation_Value_Founded = false;
			bool CabInstalled_Info_Founded = false;

			for(TU32_t j = 0 ; (j < pKey->GetValues()->Count()) && !m_Installed ; ++j)
			{
				/** Search if Key countain a CabFile Value */
				CRegValue *pValue = NULL;
				pValue = (*pKey->GetValues())[j];
				if(pValue)
				{
					pValue->Refresh();
					if(!CabLocation_Value_Founded)
					{
						if(wcscmp(pValue->Name(), CAB_LOCATION_KEY_NAME) == 0)
						{
							switch(pValue->GetValueType())
							{
								case CRegValue_Type_String_ANSI:
								{
									if(FullName() == (TS08_t)pValue->Value())
										CabLocation_Value_Founded = true;
								}
								break;
							case CRegValue_Type_String_Unicode:
								{
									if(FullName() == (TS16_t)pValue->Value())
										CabLocation_Value_Founded = true;
								}
								break;
							}
						}
					}
					if(!CabInstalled_Info_Founded)
					{
						if(wcscmp(pValue->Name(), CAB_INSTALLED_INFO_KEY_NAME) == 0)
						{
							TU32_t * pValueUL = NULL;
							switch(pValue->GetValueType())
							{
								case CRegValue_Type_32BITS_Number:
								{
									pValueUL = (TU32_t *)pValue->Value();

									if(*pValueUL == 1)
										CabInstalled_Info_Founded = true;
								}
								break;
							}
						}
					}

					if(CabInstalled_Info_Founded && CabLocation_Value_Founded)
					{
						m_Installed = true;
						memcpy(m_CabName,pKey->Name(),sizeof(*m_CabName) * wcslen(pKey->Name()));
					}
				}
			}
		}
	}
}
bool CCabFile::	Install(
bool PromptDurTInstallation
, bool CabCanBeRemoved
, bool DeleteAfterInstallation
, bool AskUserForDestinationFolder
, bool WaitUntilExit)
{
	bool Result = false;
	Result = CCommonTools::Launchprogram(	INSTCAB_PROCESS_NAME
											,MakeInstallArguments(	PromptDurTInstallation
																	, AskUserForDestinationFolder
																	, CabCanBeRemoved
																	, DeleteAfterInstallation).String()
											,WaitUntilExit);
	return Result;
}
bool CCabFile::UnInstall(bool WaitUntilExit)
{
	bool Result = false;
	CMyString<wchar_t> FileName;
	
	if(IsAllReadyInstalled())
	{
		FileName.SetString(L"%s.unload",m_CabName);
		Result = CCommonTools::Launchprogram(UNINSTCAB_PROCESS_NAME
											,FileName.String()
											,WaitUntilExit);
	}
	return Result;
}
CMyString<wchar_t> CCabFile::MakeInstallArguments(	
bool PromptDurTInstallation
, bool AskUserForDestinationFolder
, bool CabCanBeRemoved
, bool DeleteAfterInstallation)
{
	CMyString<wchar_t> ArgString;

	if(!PromptDurTInstallation)
		ArgString.AddString(L"%s ", INSTALL_CAB_ARGUMENTS_NOPROMPT);
	
	if(AskUserForDestinationFolder)
		ArgString.AddString(L"%s ", INSTALL_CAB_ARGUMENTS_ASKDEST);
	else
		ArgString.AddString(L"%s ", INSTALL_CAB_ARGUMENTS_NOASKDEST);

	if(!CabCanBeRemoved)
		ArgString.AddString(L"%s ", INSTALL_CAB_ARGUMENTS_NOUNINSTALL);

	if(!DeleteAfterInstallation)
		ArgString.AddString(L"%s %i " ,INSTALL_CAB_ARGUMENTS_DELETE, 0);
	else
		ArgString.AddString(L"%s %i " ,INSTALL_CAB_ARGUMENTS_DELETE, 1);
																			/** 0 -> Do not remove the cab file after install
																				1 -> Remove cabfile after install
																				http://msdn.microsoft.com/en-us/library/ms933760.aspx
																			*/
	/** Put cab Path And File Name */
	ArgString.AddString(L"\"%s\"", FullName().String());

	if(m_pLog)
		m_pLog->Log(NOCOMMENTS, "Cab Installation Arguments : (%w)", ArgString.String());

	return ArgString;
}