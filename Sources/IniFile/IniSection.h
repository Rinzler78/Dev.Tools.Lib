#ifndef CINI_SECTION_H
#define CINI_SECTION_H

#include "Common\MyTypes.h"
//#include "IniFile\IniSectionValue.h"
#include "LinkedList\LinkedList.h"
//#include "MyString\MyString.h"

#define SECTIONNAME_SIZE 100

TEMPLAT_TYPE_DECL
class CMyString;
class CIniSectionValue;

class CIniSection

{
public:
	CIniSection();
	CIniSection(const TS08_t  pSectionName, CLinkedList<CIniSectionValue *> *pValueList = NULL, const TS08_t pNewComment = NULL);
	CIniSection(const CIniSection &);
	~CIniSection();
	
	/** Testers */
	bool IsValueExist(const TS08_t pValueName)const;

	/** Getters */
	inline CMyString<char> & SectionName() const {return *m_pSectionName;}
	inline CMyString<char> & SectionNameComment() const {return *m_pSectionNameComment;}
	inline CLinkedList<CIniSectionValue *> & ValueList() {return m_ValueList;}

	TI32_t GetValueIndex(const TS08_t pValueName) const;
	CIniSectionValue * operator [] (const TS08_t pSectionValueName)const;
	CIniSectionValue * operator [] (const TU32_t Index)const;

	/** Setters */
	void operator += (CIniSectionValue * pSectionvalue);
	void operator += (CIniSectionValue & Sectionvalue);
	
	void ClearValueList();
private:
	CMyString<char> * m_pSectionName;
	CMyString<char> * m_pSectionNameComment;

	CLinkedList<CIniSectionValue *> m_ValueList;	///< List of values
};
#endif