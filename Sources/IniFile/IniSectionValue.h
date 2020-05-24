#ifndef CINI_SECTION_VALUE_H
#define CINI_SECTION_VALUE_H

#define VALUENAME_SIZE 100
#define VALUE_SIZE 200
#define SECTIONCOMMENT_SIZE 150

#include "Common\MyTypes.h"
#include "LinkedList\LinkedList.h"
//#include "MyString\MyString.h"

TEMPLAT_TYPE_DECL
class CMyString;

class CIniSectionValue
{
public:
	CIniSectionValue();
	CIniSectionValue(const CIniSectionValue & ObjectToCopy);
	CIniSectionValue(const TS08_t  pValueName, const TS08_t  pValue, const TS08_t  pCommentOnline = NULL);
	~CIniSectionValue();

	/** Getters */
	inline CMyString<char> & ValueName(){return *m_pValueName;}
	inline const CMyString<char> & ValueName()const{return *m_pValueName;}

	inline CMyString<char> & Value(){return *m_pValue;}
	inline const CMyString<char> & Value()const{return *m_pValue;}

	inline CMyString<char> & CommentValueOnLine(){return *m_pCommentValueOnLine;}
	inline const CMyString<char> & CommentValueOnLine()const{return *m_pCommentValueOnLine;}

	inline CLinkedList<CMyString<char> *> & CommentList(){return *m_pCommentsList;}
	inline const CLinkedList<CMyString<char> *> & CommentList()const{return *m_pCommentsList;}

	/** Setter */
	void operator = (const CIniSectionValue &);
private:
	void ClearCommentList();
	CMyString<char> * m_pValueName;
	CMyString<char> * m_pValue;
	CMyString<char> * m_pCommentValueOnLine;

	CLinkedList<CMyString<char> *> * m_pCommentsList;   ///< List of comments
};
#endif