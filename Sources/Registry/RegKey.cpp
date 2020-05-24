#ifndef CREGKEY_CPP
#define CREGKEY_CPP
#include "Registry\Regkey.h"
#include "CommonTools\CommonTools.h"
#define NAME_MAXNB_LETTER 200

CRegKey::CRegKey(HKEY RegHKey, const TS08_t  pPath) :
m_RegHKey(RegHKey)
, m_RegHKeyResult(NULL)
, m_Exist(false)
, m_Opened(false)
, m_pParentRegKey(NULL)
{
	TI32_t Index = 0;
	TC16_t NewPath [NAME_MAXNB_LETTER] = {0};

	/** Memory init */
	MACRO_MEMSET(m_Path, sizeof(m_Path))
	MACRO_MEMSET(m_Name, sizeof(m_Name))
	
	if(pPath)
	{
		/** Get Path */
		mbstowcs(m_Path,pPath,sizeof(m_Path));
		/** Get Parent Reg Key */
		Index = CCommonTools::FindLastIndex(m_Path,L'\\',0);
		if(Index > -1)
		{
			wsprintf(m_Name, L"%s",m_Path + Index + 1);
			memcpy(NewPath,m_Path,Index * sizeof(*m_Path));
			m_pParentRegKey = new CRegKey(RegHKey, NewPath);
		}
		else
			wsprintf(m_Name, L"%s",m_Path); 
	}
}
CRegKey::CRegKey(HKEY RegHKey, const TS16_t  pPath) :
m_RegHKey(RegHKey)
, m_RegHKeyResult(NULL)
, m_Exist(false)
, m_Opened(false)
, m_pParentRegKey(NULL)
{
	TI32_t Index = 0;
	TC16_t NewPath [NAME_MAXNB_LETTER] = {0};

	/** Memory init */
	MACRO_MEMSET(m_Path, sizeof(m_Path));
	MACRO_MEMSET(m_Name, sizeof(m_Name));
	
	if(pPath)
	{
		/** Get Path */
		//mbstowcs(m_Path,pPath,sizeof(m_Path));
		memcpy(m_Path, pPath, wcslen(pPath) * sizeof(pPath));
		/** Get Parent Reg Key */
		Index = CCommonTools::FindLastIndex(m_Path,L'\\',0);
		if(Index > -1)
		{
			wsprintf(m_Name, L"%s",m_Path + Index); 
			memcpy(NewPath,m_Path,Index * sizeof(*m_Path));
			m_pParentRegKey = new CRegKey(RegHKey, NewPath);
		}
		else
			wsprintf(m_Name, L"%s",m_Path); 
	}
}
CRegKey::~CRegKey()
{
	if(m_Opened)
		Close();

	while(m_ValueList.Count())
	{
		CRegValue *pValueToDelete = NULL;
		
		pValueToDelete = m_ValueList[0];

		MACRO_DELET(pValueToDelete);
		m_ValueList.DeleteAt(0);
	}

	while(m_KeyList.Count())
	{
		CRegKey *pKeyToDelete = NULL;
		
		pKeyToDelete = m_KeyList[0];

		MACRO_DELET(pKeyToDelete);
		m_KeyList.DeleteAt(0);
	}

	MACRO_DELET(m_pParentRegKey);
}
bool CRegKey::Open(TU32_t OpenAccess)
{
	if(!m_Opened)
	{
#if defined (UNDER_CE)
		m_Opened = (RegOpenKeyEx(m_RegHKey, m_Path,NULL, OpenAccess, &m_RegHKeyResult) == ERROR_SUCCESS);
#else
		m_Opened = (RegOpenKey(m_RegHKey, m_Path, &m_RegHKeyResult) == ERROR_SUCCESS);
#endif
		m_Exist = m_Opened;
	}

	return m_Opened;
}
bool CRegKey::Close()
{
	if(m_Opened)
		m_Opened = !(RegCloseKey(m_RegHKeyResult)  == ERROR_SUCCESS);

	return m_Opened;
}
bool CRegKey::Create(bool CloseAfterCreate)
{
	bool Result = false;
	
	if(!m_Exist)
	{
#if defined (UNDER_CE)
		Result = (RegCreateKeyEx(m_RegHKey
								,m_Path
								,NULL
								,NULL
								,NULL
								,NULL
								,NULL
								,&m_RegHKeyResult
								,NULL) == ERROR_SUCCESS);
#else
		Result = (RegCreateKey(m_RegHKey,m_Path,&m_RegHKeyResult) == ERROR_SUCCESS);
#endif
		m_Opened = Result;
		
		if(Result && CloseAfterCreate)
			Close();
	}

	return Result;
}
void CRegKey::Refresh()
{
	TU32_t Index = 0;
	/** For Key */
	TC16_t WName [NAME_MAXNB_LETTER] = {0};
	TC08_t Name [NAME_MAXNB_LETTER] = {0};
	DWORD NameLength = NAME_MAXNB_LETTER;
	
	if(!IsOpen())
		Open();
	if(IsOpen())
	{
		/** Populate Key List*/
		wsprintf(WName, L"%s\\",m_Path);
#if defined (UNDER_CE)
		while(RegEnumKeyEx(m_RegHKeyResult, Index++, WName + wcslen(WName), &NameLength, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			NameLength = NAME_MAXNB_LETTER;
#else
		while(RegEnumKey(m_RegHKeyResult, Index++, WName + wcslen(WName), NAME_MAXNB_LETTER) == ERROR_SUCCESS)
		{
#endif
		
			wcstombs(Name,WName,wcslen(WName));
			m_KeyList.Add(new CRegKey(m_RegHKey,Name));
			wsprintf(WName, L"%s\\",m_Path);
			MACRO_MEMSET(Name, sizeof(Name));
		}
		
		/** Populate value List*/
		Index = 0;
		while(RegEnumValue(m_RegHKeyResult, Index++, WName, &NameLength, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			if(NameLength > 0)
			{
				wcstombs(Name,WName,wcslen(WName));
				m_ValueList.Add(new CRegValue(this,Name));
				MACRO_MEMSET(Name, sizeof(Name));
				NameLength = NAME_MAXNB_LETTER;
			}
		}
		Close();
	}
}
bool CRegKey::Delete()
{
	bool Result = false;

	return Result;
}
bool CRegKey::DeleteKey(CRegKey *pRegKey)
{
	bool Result = false;
	TI32_t Index = 0;

	if(pRegKey)
	{
		Index = m_KeyList.FindFirstIndex(pRegKey);
		if(Index > 0)
		{
			pRegKey->Delete();
			m_KeyList.Delete(pRegKey);
		}
	}

	return Result;
}
bool CRegKey::Delete(CRegValue *pRegValue)
{
	bool Result = false;
	
	TI32_t Index = 0;

	if(pRegValue)
	{
		Index = m_ValueList.FindFirstIndex(pRegValue);
		if(Index > 0)
		{
			pRegValue->Delete();
			m_ValueList.Delete(pRegValue);
		}
	}

	return Result;
}
#endif