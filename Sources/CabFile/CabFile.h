#ifndef CCABFILE_H
#define CCABFILE_H
#include "FileInfo\FileInfo.h"
#include "MyString\MyString.h"
#define CAB_NAME_NB_LETTER		100

class CLog;

class CCabFile : private CFileInfo
{
public:
	CCabFile(const TS08_t  pPathAndFileName, CLog *pLog = NULL);
	CCabFile(const TS16_t  pPathAndFileName, CLog *pLog = NULL);
	~CCabFile();

	inline bool Exist()const{return Exist();}
	inline bool IsAllReadyInstalled(){return m_Installed;}

	void Refresh();
	
	bool Install(	bool PromptDurTInstallation = true
					, bool CabCanBeRemoved = true
					, bool DeleteAfterInstallation = false
					, bool AskUserForDestinationFolder = false
					, bool WaitUntilExit = true);
	bool UnInstall(bool WaitUntilExit = true);
private:
	CMyString<wchar_t> MakeInstallArguments(	bool PromptDurTInstallation = true
					, bool AskUserForDestinationFolder = false
					, bool CabCanBeRemoved = true
					, bool DeleteAfterInstallation = true);

	TC16_t m_CabName [CAB_NAME_NB_LETTER];	///< .
	bool m_Installed;   ///< true if installed
	CLog * m_pLog;
};
#endif