#include "ConfigFile.h"
#include "IniFile\IniFile.h"
#include "Log\Log.h"
#include "MyString\MyString.h"
CConfigFile::CConfigFile(const TS08_t pIniPathAndFileName, CLog * pLog)
: m_pIniFile(NULL)
, m_LoadedSuccess(false)
, m_pLog(pLog)
, m_FileCorrupted(false)
{
	m_pIniFile = new CIniFile(pIniPathAndFileName);
	Load();
}
CConfigFile::~CConfigFile()
{
	MACRO_DELET(m_pIniFile)
}
bool CConfigFile::UpdateConfigFile()
{
	bool Result = false;
	
	if(m_pIniFile)
		Result = m_pIniFile->UpdateFile();
	return Result;
}
void CConfigFile::Load()
{
	if(m_pIniFile)
	{
		if(m_pIniFile->Exist())
		{
			m_LoadedSuccess = m_pIniFile->Load();
			if(m_pIniFile->IsLoaded())
			{
				if(m_pLog)
					m_pLog->Log(NOCOMMENTS, "To Do File %s load Success", m_pIniFile->FullName().String());
			}
			else
			{
				if(m_pLog)
					m_pLog->Log(ALERT, "To Do File %s not loaded.", m_pIniFile->FullName().String());
			}
		}
		else
		{
			if(m_pLog)
				m_pLog->Log(ALERT, "To Do File %s does not exist. Set Default Content in File", m_pIniFile->FullName().String());
		}
	}
}