#ifndef CFILESYSTEMINFO_H
#define CFILESYSTEMINFO_H
#include "Common\MyTypes.h"

#define FULLNAME_MAX_NB_LETTER 200
#define NAME_MAX_NB_LETTER 200
#define FILEEXTENSION_MAX_NB_LETTER 10

class CDirectoryInfo;
class CLog;
TEMPLAT_TYPE_DECL
class CMyString;

class CFileSystemInfo
{
public: 
	CFileSystemInfo(const TS08_t pPathAndFileName, CLog *pLog = NULL);
	CFileSystemInfo(const CMyString<char> & PathAndFileName, CLog *pLog = NULL);
	~CFileSystemInfo();
	
	/** Parent Directory */
	CDirectoryInfo * GetParentDirectory() const {return m_pParentDirectory;}

	/** FullName, Name, Extension */
	const CMyString<char> & Name() const {return *m_pName;};
	const CMyString<char> & FullName() const {return *m_pFullName;};
	const CMyString<char> & Extension()const {return *m_pExtension;};
	
	/** Getters */
	inline bool Exist() const {return m_Exist;}
	inline TU64_t Size() const{return m_Size;}
	inline TU16_t GetCRC() const{return m_CRC16;};
	
	/** Virtual Mebers */
	virtual void Refresh();
	virtual bool RetrieveCRC16(){return false;};
	virtual bool Create(){return false;};
	virtual bool Delete(){return false;};
	virtual bool CopyTo(const TS08_t pPath, bool EraseIfExist = false){return false;};
	virtual bool MoveTo(const TS08_t pPath, bool EraseIfExist = false){return false;};

	static CMyString<char> ResolvePath(const TS08_t PathAndFileName);
	static CMyString<char> GetCurrent_Directory();
	static CMyString<char> GetCurrent_AppName();
	static CMyString<char> GetCurrent_AppFullPath();
protected:
	/** Optionnal Log */
	CLog *m_pLog;   ///< The log
	bool m_Exist;   ///< true to exist
	TU64_t m_Size;	///< The size

	CMyString<char> * m_pName;
	CMyString<char> * m_pFullName;
	CMyString<char> * m_pExtension;

	/** Directory */
	CDirectoryInfo * m_pParentDirectory;	///< Pathname of the parent directory
	
	/** CRC16 */
	TU16_t m_CRC16;   ///< The crc 16

private:
	void SetFileSystemInfo(const TS08_t pPathAndFileName);
};
#endif