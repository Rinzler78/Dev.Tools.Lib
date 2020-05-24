#ifndef CFILEINFO_H
#define CFILEINFO_H
#define FILEPATH_MAX_NB_LETTER 50

#include "Common\MyTypes.h"
#include <iostream>
#include "FileSystemInfo\FileSystemInfo.h"

typedef enum FileOpen_Attribute
{
	FileOpen_Initvalue = 0, /*!< Default value */
	OPEN_IN_READ_ATTRIBUTE, /*!< Open a file in Reading */
	OPEN_IN_WRITE_ATTRIBUTE,/*!< Open a file in Writing*/
	OPEN_FOR_APPEND_ATTRIBUTE,/*!< Open a file to Append data at the end */
	OPEN_BOTH_READ_WRITE_ATTRIBUTE,/*!< Open in reading and writing*/
	OPEN_IN_WRITE_BOTH_READ_WRITE_ATTRIBUTE,/*!< Open in reading and writing*/
	OPEN_BOTH_READ_APPEND_ATTRIBUTE,/*!< Open in reading and Appening */
	OPEN_IN_READ_ATTRIBUTE_AS_BINARY,/*!< Open a binary file in Reading */
	OPEN_IN_WRITE_ATTRIBUTE_AS_BINARY,/*!< Open a binary file in Writing */
	OPEN_FOR_APPEND_ATTRIBUTE_AS_BINARY,/*!< Open a binary file in Appening */
	OPEN_BOTH_READ_WRITE_ATTRIBUTE_AS_BINARY,/*!< Open a binary file in Reading and Writing */
	OPEN_IN_WRITE_BOTH_READ_WRITE_ATTRIBUTE_AS_BINARY,/*!< Open a binary file in Reading and Writing */
	OPEN_BOTH_READ_APPEND_ATTRIBUTE_AS_BINARY,/*!< Open a binary file in Reading and Appening */
	MAX_OPEN_ATTRIBUT/*!< Max value of enum*/
}FileOpen_Attribute_t;

class CDirectoryInfo;
class CLog;
TEMPLAT_TYPE_DECL
class CMyString;

class CFileInfo : public CFileSystemInfo
{
public:
	CFileInfo(const TS08_t pPathAndFileName, CLog *pLog = NULL);
	CFileInfo(const CMyString<char> & PathAndFileName, CLog *pLog = NULL);
	~CFileInfo();

	/** Getters */
	inline bool IsOpened() const{return m_FileOpened;}
	inline TS08_t GetContent() const{return m_pFileContent;}
	
	/** File Content */
	void Refresh();
	TU64_t ReadContent(TS08_t Out_Buffer, TU64_t MaxSize, TU32_t StartIndex = 0);
	bool LoadContent();

	/** File Modification */
	bool WriteLine(const TS08_t  pCharStringToWrite,...);
	bool Write(const TS08_t  pCharStringToWrite, bool Auto_Close = true);

	bool CopyTo(const TS08_t  pPathAndFileName, bool EraseIfExist = false);
	bool MoveTo(const TS08_t  pPathAndFileName, bool EraseIfExist = false);
	
	bool Create();
	bool Delete();
	
	bool Open(FileOpen_Attribute_t OpenAtt);
	bool Close();
	ING_STATUS_t Execute(const TS08_t  pArguments = NULL, bool WaitUntilExit = false);
	
	/** Auto Close Lock */
	inline void LockAutoClose() {m_BlockAutoClose = true;}
	inline void UnLockAutoClose() {m_BlockAutoClose = false;}
	inline bool AutoCloseLocked()const {return m_BlockAutoClose;}

	/** CRC */
	bool RetrieveCRC16();
private:
	TU64_t GetFileSize(bool CloseFileAfter = true);
	bool m_FileOpened;  ///< true if file opened
	FileOpen_Attribute_t m_FileCurrentOpeningAttribut;  ///< The file current opening attribut
	FILE * m_pFile; ///< The file

	TC08_t * m_pFileContent;  ///< The file content
	bool m_BlockAutoClose;  ///< true to block automatic close
};
#endif