#ifndef CINI_FILE_H
#define CINI_FILE_H
#include "Common\MyTypes.h"
//#include "LinkedList\LinkedList.h"
//#include "FileInfo\FileInfo.h"

class CIniSection;
class CIniSectionValue;
class CLog;
TEMPLAT_DECL
class CLinkedList;
TEMPLAT_TYPE_DECL
class CMyString;
class CFileInfo;

class CIniFile
{
public:
	CIniFile(const TS08_t  pIniPathAndFileName, CLog *pLog = NULL);
	~CIniFile();

	/** Getters */
	bool IsLoaded() const;
	bool Exist() const;
	TU64_t Size() const;
	const CMyString<char> & FullName()const;
	inline CLinkedList<CIniSection *> & SectionList()const{return *m_pSectionList;}
	inline CLinkedList<CMyString<char> *> & CommentList()const{return *m_pCommentList;}
	TI32_t GetSectionIndex(const TS08_t pSectionName) const;
	CIniSection * operator [] (const TS08_t pSectionName) const;
	CIniSection * operator [] (const TU32_t Index) const;
	
	/** Setters */
	bool SetContentInFile(CLinkedList<CMyString<char> *> * pCommentList, CLinkedList<CIniSection *> * pSectionList);
	bool SetContentInFile(const TS08_t pCharStringToPutInfile);
	bool UpdateFile();
	bool Delete();

	/** Loading */
	bool Load();
	bool Load(const TS08_t  pIniPathAndFileName);
private:
	bool UpdateFile(CLinkedList<CMyString<char> *> * pCommentList, CLinkedList<CIniSection *> * pSectionList);
	static CIniSection GetSectionInLine(TS08_t  pLine, TU32_t Offset = 0);
	static CIniSectionValue GetValueInLine(const TS08_t  pLine, TU32_t Offset = 0);
	static CMyString<char> GetComment(const TS08_t  pLine, TU32_t Offset = 0);

	bool RetrieveInfoFromFile();
	bool Parse(const TS08_t FileContent);
	void ClearCommentList();
	void ClearSectionList();

	CLinkedList<CMyString<char> *> * m_pCommentList;	///< List of comments
	CLinkedList<CIniSection *> * m_pSectionList;   ///< List of sections
	CFileInfo * m_pFileInfo;	///< Information describing the file
	CLog *m_pLog;   ///< The log
	bool m_Loaded;  ///< true if the data was loaded
};
#endif