#include "CRCReportFile.h"
#include "IniFile\IniFile.h"
#include "IniFile\IniSection.h"
#include "IniFile\IniSectionValue.h"
#include "CommonTools\CommonTools.h"
#include "DirectoryInfo\DirectoryInfo.h"
#include <stdio.h>
#include <stdlib.h>
#include "Log\Log.h"
#include "MyString\MyString.h"
#include "FileInfo\FileInfo.h"

CRCReportFile::CRCReportFile(const TS08_t  pIniPathAndFileName, CLog *pLog)
:m_pIniFile(NULL)
, m_LoadedSuccess(false)
, m_GlobalCRC(0)
, m_pLog(pLog)
, m_pDirectoryOrFileToSkipSection(NULL)
, m_pCRCDetailsSection(NULL)
, m_pDirectoryReference(new CMyString<char>())
{
	m_pIniFile = new CIniFile(pIniPathAndFileName,m_pLog);
	Load();
}
CRCReportFile::~CRCReportFile()
{
	MACRO_DELET(m_pIniFile)
	MACRO_DELET(m_pDirectoryReference)
}
void CRCReportFile::SetReferenceDirectory(TS08_t pNewReferenceDirectory)
{
	MACRO_DELET(m_pDirectoryReference)
	m_pDirectoryReference = new CMyString<char>(pNewReferenceDirectory);
}
bool CRCReportFile::AddSkipDirectory(TS08_t pPath)
{
	bool Result = false;
	
	if(pPath)
	{
		if(m_pIniFile)
		{
			if(!m_pDirectoryOrFileToSkipSection)
			{	
				m_pDirectoryOrFileToSkipSection = new CIniSection();
				m_pDirectoryOrFileToSkipSection->SectionName() = DEFAULT_CRC_REPORT_SECTION_NAME_2_;
				m_pIniFile->SectionList().InsertAt(1, m_pDirectoryOrFileToSkipSection);
			}
			
			if(m_pDirectoryOrFileToSkipSection)
			{
				CMyString<char> ValueName;
				CIniSectionValue * pSectionvalue = NULL;

				/** Value Allready Exist ? */
				for(TU32_t i = 0 ; i < m_pDirectoryOrFileToSkipSection->ValueList().Count() ; ++i)
				{
					ValueName.SetString("S%u", i);
					
					pSectionvalue = (*m_pDirectoryOrFileToSkipSection)[ValueName.String()];
					if(pSectionvalue)
					{
						if((*pSectionvalue).Value() == pPath)
						{
							/** Value Allready Exist */
							break;
						}
					}
					pSectionvalue = NULL;
				}
				
				if(!pSectionvalue)
				{
					/** Value Not Exist So Add To List */
					pSectionvalue = new CIniSectionValue();
					
					pSectionvalue->ValueName().SetString("S%u", m_pDirectoryOrFileToSkipSection->ValueList().Count());
					(*pSectionvalue).Value() = pPath;
					Result = m_pDirectoryOrFileToSkipSection->ValueList().Add(pSectionvalue);
				}
			}
		}
	}
	return Result;
}
bool CRCReportFile::IsMustbeSkip(TS08_t StringToSearch) const
{
	bool Result = false;
	
	if(StringToSearch)
	{
		if(m_pDirectoryOrFileToSkipSection)
		{
			for(TU32_t i = 0 ; i < m_pDirectoryOrFileToSkipSection->ValueList().Count() ; ++i)
			{
				CIniSectionValue * pSectionValue = NULL;
				
				pSectionValue = m_pDirectoryOrFileToSkipSection->ValueList()[i];
				
				if(pSectionValue->Value() == StringToSearch)
				{
					Result = true;
					break;
				}
			}
		}
	}
	return Result;
}
void CRCReportFile::UpdateDetails()
{
	CDirectoryInfo ReferenceDirectory(m_pDirectoryReference->String());
	if(!m_pCRCDetailsSection)
		m_pCRCDetailsSection = new CIniSection(DEFAULT_CRC_REPORT_SECTION_NAME_3_);

	m_pCRCDetailsSection->ClearValueList();
	
	if(m_pLog)
		m_pLog->Log(NOCOMMENTS, "UpdateDetails on %s", m_pDirectoryReference->String());
	//ReferenceDirectory.Refresh(); /** Refresh childs (directories and files) */
	
	TU32_t DirectoryIndex = 0;
	GetCRCDetails(DirectoryIndex, &ReferenceDirectory);
	
	ReferenceDirectory.RetrieveCRC16();
	m_GlobalCRC = ReferenceDirectory.GetCRC();

	m_NbCRCDetails = m_pCRCDetailsSection->ValueList().Count();

	if(m_pCRCDetailsSection)
		m_NbReferenceDetails = m_pCRCDetailsSection->ValueList().Count();

	if(m_pLog)
		m_pLog->Log(NOCOMMENTS, "End of UpdateDetails on %s", m_pDirectoryReference->String());
}
bool CRCReportFile::GetCRCDetails(TU32_t &IndexNumber, CDirectoryInfo *pDirectoryToScan)
{
	bool Result = false;
	CIniSectionValue *pSectionvalue = NULL;
	TU32_t Directory_Index = IndexNumber;
	TU32_t InsertAt = 0;

	if(!m_pCRCDetailsSection)
		m_pCRCDetailsSection = new CIniSection(DEFAULT_CRC_REPORT_SECTION_NAME_3_);
	
	if(pDirectoryToScan && m_pCRCDetailsSection)
	{
		pDirectoryToScan->Refresh();

		for(TU32_t i = 0 ; i < pDirectoryToScan->GetDirectories().Count() ; ++i)
		{
			CDirectoryInfo *pCurrentDirectory = pDirectoryToScan->GetDirectories()[i];
			if(pCurrentDirectory)
			{
				if(!IsMustbeSkip(pCurrentDirectory->FullName().String()))
				{
					++IndexNumber;
					GetCRCDetails(IndexNumber, pCurrentDirectory);
					continue; /** Skip Directory Delete */
				}

				MACRO_DELET(pCurrentDirectory);
			}
			pDirectoryToScan->GetDirectories().DeleteAt(i);
			--i;
		}

		
		for(TU32_t i = 0 ; i < pDirectoryToScan->GetFiles().Count() ; ++i)
		{
			CFileInfo *pCurrentFile = pDirectoryToScan->GetFiles()[i];
			if(pCurrentFile)
			{
				if(!IsMustbeSkip(pCurrentFile->FullName().String()))
				{
					++IndexNumber;
					pSectionvalue = new CIniSectionValue();
					
					pSectionvalue->ValueName().SetString("F%u",IndexNumber);
					pCurrentFile->RetrieveCRC16();

					(*pSectionvalue).Value().SetString("%s CRC->%u",pCurrentFile->FullName().String(), pCurrentFile->GetCRC());
					
					m_pCRCDetailsSection->ValueList().Add(pSectionvalue);
					continue; /** Skip File Delete */
				}
				MACRO_DELET(pCurrentFile);
			}
			pDirectoryToScan->GetFiles().DeleteAt(i);
			--i;
		}
		
		pDirectoryToScan->RetrieveCRC16();
		pSectionvalue = new CIniSectionValue();
		
		InsertAt = m_pCRCDetailsSection->ValueList().Count() - (IndexNumber - Directory_Index);

		pSectionvalue->ValueName().SetString("D%u",Directory_Index);
		pSectionvalue->Value().SetString("%s CRC->%u",pDirectoryToScan->FullName().String(), pDirectoryToScan->GetCRC());

		m_pCRCDetailsSection->ValueList().InsertAt(InsertAt, pSectionvalue);
	}

	return Result;
}
void CRCReportFile::Load()
{
	bool ValueMissing = false;
	if(m_pIniFile && m_pIniFile->Exist())
	{
		if(m_pIniFile->Load())
		{
			/** Load Success -> Read Configuration */
			CIniSection *pSection = NULL;
			CIniSectionValue *pSectionValue = NULL;
			
			/** Directory Reference Section */
			if(pSection = (*m_pIniFile)[DEFAULT_CRC_REPORT_SECTION_NAME_1_])
			{
				/** Refrence Directory Value */
				if(pSectionValue = (*pSection)[DEFAULT_CRC_REPORT_SECTION_NAME_1_REF_DIRECTORY])
				{
					(*m_pDirectoryReference) = (*pSectionValue).Value();
				}
				else
					ValueMissing = true;

				/** Global CRC Value */
				if(pSectionValue = (*pSection)[DEFAULT_CRC_REPORT_SECTION_NAME_1_GLOBAL_CRC])
				{
					m_GlobalCRC = atoi((*pSectionValue).Value().String());
				}
				else
					ValueMissing = true;

				/** Nb Details in report Value */
				if(pSectionValue = (*pSection)[DEFAULT_CRC_REPORT_SECTION_NAME_1_NB_DETAILS])
				{
					m_NbReferenceDetails = atoi((*pSectionValue).Value().String());
				}
				else
					ValueMissing = true;
			}
			else
				ValueMissing = true;
			
			if(ValueMissing)
				goto ValueMissing_For_Goto;

			if(!(m_pDirectoryOrFileToSkipSection = (*m_pIniFile)[DEFAULT_CRC_REPORT_SECTION_NAME_2_]))
				ValueMissing = true;
				
			if(ValueMissing)
				goto ValueMissing_For_Goto;

			if((m_pCRCDetailsSection = (*m_pIniFile)[DEFAULT_CRC_REPORT_SECTION_NAME_3_]))
			{
				m_NbCRCDetails = m_pCRCDetailsSection->ValueList().Count();
			}
			else
				ValueMissing = true;
ValueMissing_For_Goto:
			if(ValueMissing)
			{
				if(m_pLog)
					m_pLog->Log(ALERT,"Value Missing, Set Default content in file");
				m_pIniFile->SetContentInFile(Default_CRCReportFile);
				//Load();
			}
			else
				m_LoadedSuccess = true;
		}
		else
		{
			/** Load Failure */
			if(m_pLog)
				m_pLog->Log(ALERT,"Ini file %s Load failure", m_pIniFile->FullName().String());
			m_pIniFile->SetContentInFile(Default_CRCReportFile);
			//Load();
		}
	}
	else
	{
		/** Create an ini file with default content and */
		if(m_pLog)
			m_pLog->Log(ALERT,"Ini file %s does not exist", m_pIniFile->FullName().String());
		m_pIniFile->SetContentInFile(Default_CRCReportFile);
		//Load();
	}
}
bool CRCReportFile::SaveReport(TS08_t *pNewDestination)
{
	bool Result = false;
	CMyString<char> TempString;
	
	if(m_pIniFile)
	{
		if(m_pLog)
			m_pLog->Log(NOCOMMENTS, "Save report to %s", m_pIniFile->FullName().String());

		CIniSection *pSection = NULL;
		CIniSectionValue *pSectionValue = NULL;

		pSection = (*m_pIniFile)[DEFAULT_CRC_REPORT_SECTION_NAME_1_];

		if(!pSection)
		{
			pSection = new CIniSection(DEFAULT_CRC_REPORT_SECTION_NAME_1_);
			m_pIniFile->SectionList().InsertAt(0, pSection);
		}

		if(pSection)
		{
			/** Reference Directory */
			pSectionValue = (*pSection)[DEFAULT_CRC_REPORT_SECTION_NAME_1_REF_DIRECTORY];

			if(!pSectionValue)
			{
				pSectionValue = new CIniSectionValue();
				pSectionValue->ValueName() = DEFAULT_CRC_REPORT_SECTION_NAME_1_REF_DIRECTORY;
				pSection->ValueList().Add(pSectionValue);
			}
			if(pSectionValue)
				pSectionValue->Value() = m_pDirectoryReference;

			/** Global CRC */
			pSectionValue = (*pSection)[DEFAULT_CRC_REPORT_SECTION_NAME_1_GLOBAL_CRC];

			if(!pSectionValue)
			{
				pSectionValue = new CIniSectionValue();
				pSectionValue->ValueName() = DEFAULT_CRC_REPORT_SECTION_NAME_1_GLOBAL_CRC;
				pSection->ValueList().Add(pSectionValue);
			}

			if(pSectionValue)
				pSectionValue->Value().SetString("%u",m_GlobalCRC);
			
			/** Nb Details */
			pSectionValue = (*pSection)[DEFAULT_CRC_REPORT_SECTION_NAME_1_NB_DETAILS];

			if(!pSectionValue)
			{
				pSectionValue = new CIniSectionValue();
				pSectionValue->ValueName() = DEFAULT_CRC_REPORT_SECTION_NAME_1_NB_DETAILS;
				pSection->ValueList().Add(pSectionValue);
			}

			if(pSectionValue)
				pSectionValue->Value().SetString("%u",m_NbReferenceDetails);			
		}
		
		pSection = (*m_pIniFile)[DEFAULT_CRC_REPORT_SECTION_NAME_3_];
		if(!pSection && m_pCRCDetailsSection)
				m_pIniFile->SectionList().Add(m_pCRCDetailsSection);
		
		Result = m_pIniFile->UpdateFile();
	}

	return Result;
}