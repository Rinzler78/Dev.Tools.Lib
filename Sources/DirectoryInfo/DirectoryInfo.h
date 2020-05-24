#ifndef CDIRECTORYINFO_H
#define CDIRECTORYINFO_H

#include "Common\MyTypes.h"
#include "FileSystemInfo\FileSystemInfo.h"

class CFileInfo;
class CLog;
TEMPLAT_DECL
class CLinkedList;
TEMPLAT_TYPE_DECL
class CMyString;

class CDirectoryInfo : public CFileSystemInfo
{
public:
	CDirectoryInfo(const TS08_t  pPathDirectory, CLog *pLog = NULL);
	CDirectoryInfo(const CMyString<char> & PathDirectory, CLog *pLog = NULL);
	~CDirectoryInfo();

	/** Setters */
	bool Create();
	bool Delete();

	/** Getters */
	inline CLinkedList<CDirectoryInfo *> & GetDirectories()const {return *m_pChildDirectoryList;}
	void Refresh();
	inline CLinkedList<CFileInfo *> & GetFiles(){return *m_pChildFilesList;}
	bool CopyTo(const TS08_t  pPath, bool EraseIfExist = false);
	bool MoveTo(const TS08_t  pPath, bool EraseIfExist = false);
	bool RetrieveCRC16();
private:
	void ClearDirectorylist();
	void ClearFilelist();
	/** Child Directory */
	CLinkedList<CDirectoryInfo *> * m_pChildDirectoryList; ///< List of child directories
	/** Chile Files */
	CLinkedList<CFileInfo *> * m_pChildFilesList;  ///< List of child files
};
#endif