#ifndef CCONFIGFILE_H
#define CCONFIGFILE_H
#include "Common\MyTypes.h"
class CIniFile;
class CLog;

class CConfigFile
{
public:
	CConfigFile(const TS08_t pIniPathAndFileName, CLog * pLog = NULL);
	~CConfigFile();
	inline CIniFile * IniFile(){return m_pIniFile;}
	inline bool Load_Success(){return m_LoadedSuccess;}
	inline bool FileCorrupted(){return m_FileCorrupted;}
	bool UpdateConfigFile();
protected:
	void Load();
	CIniFile * m_pIniFile;  ///< The initialise file
	bool m_LoadedSuccess;   ///< true if the loaded operation was a success, false if it failed
	CLog * m_pLog;  ///< The log
	bool m_FileCorrupted;   ///< true if file corrupted
};
#endif