#ifndef IPA_AutoInstaller_CRCFile_h
#define IPA_AutoInstaller_CRCFile_h
#include "Common\MyTypes.h"
#include "LinkedList\LinkedList.h"

/** IPA AutoInstaller Default File Content */
#define DEFAULT_CRC_REPORT_SECTION_NAME_1_ "Reference Base"
#define DEFAULT_CRC_REPORT_SECTION_NAME_1_REF_DIRECTORY "Reference Directory"
#define DEFAULT_CRC_REPORT_SECTION_NAME_1_GLOBAL_CRC "Global CRC"
#define DEFAULT_CRC_REPORT_SECTION_NAME_1_NB_DETAILS "Number of Details"
#define DEFAULT_CRC_REPORT_SECTION_NAME_2_ "Directory or File To Skip"
#define DEFAULT_CRC_REPORT_SECTION_NAME_3_ "CRC Details"

const TS08_t Default_CRCReportFile = \
";IPA Auto Installer CRC file Report\n\
["DEFAULT_CRC_REPORT_SECTION_NAME_1_"]\n\
;Directory Start point of CRC Calculation\n\
"
#if defined (UNDER_CE)
DEFAULT_CRC_REPORT_SECTION_NAME_1_REF_DIRECTORY"=\\\n\
"
#else
DEFAULT_CRC_REPORT_SECTION_NAME_1_REF_DIRECTORY"=c:\\\n\
"
#endif
";Global CRC in the reference directory\n\
"DEFAULT_CRC_REPORT_SECTION_NAME_1_GLOBAL_CRC"=0\n\
;Number Of Details\n\
"DEFAULT_CRC_REPORT_SECTION_NAME_1_NB_DETAILS"=0\n\
["DEFAULT_CRC_REPORT_SECTION_NAME_2_"]\n\
;S0=\\Storage Card\n\
;S1=\\Program Files\\desktop.ini\n\
;SN=...\n\
"
#if defined (UNDER_CE)
"S0=\\Storage Card\n\
S1=\\My Documents\n\
S2=\\Windows\\www\\current-httpd.log\n\
S3=\\Documents and Settings\\default.mky\n\
S4=\\Control Panel.lnk\n\
"
#else

#endif
"\n\
["DEFAULT_CRC_REPORT_SECTION_NAME_3_"]\n\
;Enumeration of Directories and files followed by CRC\n\
;D0=\\C3 CRC->0\n\
;F0=\\C3\\c3config CRC->0\n\
;FN=... CRC->0\n\
;DN=... CRC->0\n\
";

#define TIMEZONE_MAX_NB_LETTER 6 

class CIniFile;
class CDirectoryInfo;
class CLog;
class CIniSection;
TEMPLAT_TYPE_DECL
class CMyString;

class CRCReportFile
{
public:

	CRCReportFile(const TS08_t  pIniPathAndFileName, CLog *pLog = NULL);
	~CRCReportFile();

	/** Getters */
	inline CIniFile & IniFile()const {return *m_pIniFile;}
	inline bool Load_Success()const{return m_LoadedSuccess;}
	inline CMyString<char> & ReferenceDirectory() const {return *m_pDirectoryReference;}
	inline TU16_t GlobalCRC() const {return m_GlobalCRC;}
	inline TU32_t NumberOfReferenceDetails() const {return m_NbReferenceDetails;} /** Number in reference base section */
	inline TU32_t NumberOfCRCDetails() const {return m_NbCRCDetails;} /** Number of value in CRC Details section */

	bool IsMustbeSkip(TS08_t StringToSearch) const;
	void UpdateDetails();
	bool GetCRCDetails(TU32_t &IndexNumber, CDirectoryInfo *pDirectoryToScan);
	bool SaveReport(TS08_t *pNewDestination = NULL);
	
	void SetReferenceDirectory(TS08_t pNewReferenceDirectory);
	
	bool AddSkipDirectory(TS08_t pPath);
	
	void Load();
private:
	
	CIniFile *m_pIniFile;   ///< The initialise file
	bool m_LoadedSuccess;   ///< true if the loaded operation was a success, false if it failed
	CMyString<char> * m_pDirectoryReference;	///< .
	TU16_t m_GlobalCRC;   ///< The global crc
	TU32_t m_NbReferenceDetails;  ///< The nb reference details
	TU32_t m_NbCRCDetails;	///< The nb crc details
	CLog *m_pLog;   ///< The log
	CIniSection * m_pDirectoryOrFileToSkipSection;  ///< The directory or file to skip section
	CIniSection * m_pCRCDetailsSection; ///< The crc details section
};
#endif