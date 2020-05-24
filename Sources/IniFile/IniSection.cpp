#ifndef CINI_SECTION_CPP
#define CINI_SECTION_CPP
#include "IniSection.h"
#include "windows.h"
#include "LinkedList\LinkedList.h"
#include "MyString\MyString.h"
#include "IniFile\IniSectionValue.h"

CIniSection::CIniSection()
:m_pSectionName(new CMyString<char>())
, m_pSectionNameComment(new CMyString<char>())
{
}

CIniSection::CIniSection(const TS08_t  pSectionName, CLinkedList<CIniSectionValue *> *pValueList, const TS08_t pNewComment)
:m_pSectionName(new CMyString<char>(pSectionName))
, m_pSectionNameComment(new CMyString<char>(pNewComment))
{
	if(pValueList)
		m_ValueList.AddRange((CLinkedList<CIniSectionValue *> *)pValueList);
}

CIniSection::CIniSection(const CIniSection & SectionSrc)
:m_pSectionName(new CMyString<char>(SectionSrc.SectionName()))
, m_pSectionNameComment(new CMyString<char>(SectionSrc.SectionNameComment()))
{
}
CIniSection::~CIniSection()
{
	MACRO_DELET(m_pSectionName)
	MACRO_DELET(m_pSectionNameComment)
	ClearValueList();
}
bool CIniSection::IsValueExist(const TS08_t pValueName)const
{
	bool Result = false;
	
	if(pValueName)
	{
		for(TU32_t  i = 0 ; i < m_ValueList.Count() ; ++i)
		{
			if(SectionName() == pValueName)
			{
				Result = true;
				break;
			}
		}
	}

	return Result;
}
void CIniSection::ClearValueList()
{
	/** Free Value List */
	while(m_ValueList.Count())
	{
		CIniSectionValue *pValue = NULL;

		pValue = m_ValueList[0];
		m_ValueList.DeleteAt(0);

		MACRO_DELET(pValue);
	}
}
TI32_t CIniSection::GetValueIndex(const TS08_t pValueName) const
{
	TI32_t Result = -1;
	
	if(pValueName)
	{
		for(TU32_t  i = 0 ; i < (TU32_t)m_ValueList.Count() ; ++i)
		{
			if(m_ValueList[i]->ValueName() == pValueName)
			{
				Result = i;
				break;
			}
		}
	}

	return Result;
}
CIniSectionValue * CIniSection::operator [] (const TS08_t pSectionValueName)const
{
	CIniSectionValue *pResult = NULL;

	if(pSectionValueName)
	{
		TI32_t Index = GetValueIndex(pSectionValueName);
		if(Index > -1)
			pResult = m_ValueList[Index];
	}
	return pResult;
}
CIniSectionValue * CIniSection::operator [] (const TU32_t Index)const
{
	return m_ValueList[Index];
}
void CIniSection::operator += (CIniSectionValue * pSectionvalue)
{
	if(pSectionvalue)
		m_ValueList.Add(pSectionvalue);
}
void CIniSection::operator += (CIniSectionValue & Sectionvalue)
{
	CIniSectionValue * pSectionvalue = NULL;
	pSectionvalue = new CIniSectionValue(Sectionvalue);
	m_ValueList.Add(pSectionvalue);
}
#endif