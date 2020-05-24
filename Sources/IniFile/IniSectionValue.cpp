#ifndef CINI_SECTION_VALUE_CPP
#define CINI_SECTION_VALUE_CPP
#include "IniFile\IniSectionValue.h"
#include "windows.h"
#include "stdio.h"
#include "MyString\MyString.h"
//#include "LinkedList\LinkedList.h"

CIniSectionValue::CIniSectionValue()
:m_pValueName(new CMyString<char>())
, m_pValue(new CMyString<char>())
, m_pCommentValueOnLine(new CMyString<char>())
, m_pCommentsList(new CLinkedList<CMyString<char> *>())
{
}
CIniSectionValue::CIniSectionValue(const CIniSectionValue & ObjectToCopy)
:m_pValueName(new CMyString<char>(ObjectToCopy.ValueName()))
, m_pValue(new CMyString<char>(ObjectToCopy.Value()))
, m_pCommentValueOnLine(new CMyString<char>(ObjectToCopy.CommentValueOnLine()))
, m_pCommentsList(new CLinkedList<CMyString<char> *>())
{
}
CIniSectionValue::CIniSectionValue(const TS08_t  pValueName, const TS08_t  pValue, const TS08_t pCommentOnline)
:m_pValueName(new CMyString<char>(pValueName))
, m_pValue(new CMyString<char>(pValue))
, m_pCommentValueOnLine(new CMyString<char>(pCommentOnline))
, m_pCommentsList(new CLinkedList<CMyString<char> *>())
{
}

CIniSectionValue::~CIniSectionValue()
{
	MACRO_DELET(m_pValueName)
	MACRO_DELET(m_pValue)
	MACRO_DELET(m_pCommentValueOnLine)
	ClearCommentList();
	MACRO_DELET(m_pCommentsList)
}
void CIniSectionValue::ClearCommentList()
{
	while(CommentList().Count())
	{
		CMyString<char> * pComment = NULL;
		pComment = CommentList()[0];
		CommentList().DeleteAt(0);
		MACRO_DELET(pComment)
	}
}
void CIniSectionValue::operator = (const CIniSectionValue & Src)
{
	ValueName() = Src.ValueName();
	Value() = Src.Value();
	CommentValueOnLine() = Src.CommentValueOnLine();
}
#endif