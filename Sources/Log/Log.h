#ifndef CLOG_H
#define CLOG_H
#include "Common\MyTypes.h"
#include "common\common.h"
#include "LinkedList\LinkedList.h"

/** File Info */
#define PATH_MAX_SIZE 100
#define FILE_NAME_MAX_SIZE 100
#define MAX_NB_CHAR_PER_LINE_IN_LOG 250

typedef enum Log_type{
	NOCOMMENTS = 0, /*!< No comment put between date and log string*/
	NORMAL_USE, /*!< "NORMAL" will be put between date and log string*/
	ALERT, /*!< "ALERT" will be put between date and log string*/
	ERROR_Log, /*!< "ERROR" will be put between date and log string*/
	NB_MAX_LOG_TYPE /*!< Max value of Log type*/
}Log_type_t;	///< .

typedef enum Log_Output
{
	LOG_OUT_FILE = 1, /*!< Log Will be Write in log file */
	LOG_OUT_CONSOLE = 2, /*!< Log Will be Write on console */
	LOG_OUT_PRINT = 4, /*!< Log Will be Write on printer -> Not implemented yet */
	LOG_OUT_SPECIFIED_OUT = 8,
	MAX_LOG_OUT /*!< max value of Log Output */
}Log_Output_t;  ///< .

/** Following fields cannot be implemented in C++/Cli */
const TC08_t Log_type_CharTab [NB_MAX_LOG_TYPE][10] = {"","NORMAL","ALERT","ERROR"};  ///< The 10]
const TC08_t Log_Output_CharTab [MAX_LOG_OUT][10] = {"",">FILE",">CONSOLE","",">PRINT"};  ///< The 10]
typedef void (*pSpecifiedOutCallBack)(void * pToObj, TS08_t LogString, TU32_t Len);

class CThread;
class CMutex;
class CFileInfo;
TEMPLAT_TYPE_DECL
class CMyString;

class CLog
{
	public:
		CLog();
		CLog(const TS08_t pPathAndFilename);
		~CLog();
		
		void Active_Debug(bool NewFile = false, TU08_t LogOut = LOG_OUT_FILE, bool DateAtLineBegin = true, bool AppNameLineBegin = true);
		inline void DesActive_Debug(){m_LogActivated = false; m_LogOutput = 0;}

		bool SetPathAndFileName(const TS08_t pPathAndFilename,...);
		TS08_t FileName();
		void Log(Log_type_t LogType, const TS08_t  pComments, ...);
		static bool LogToConsole(const TS08_t  pComments);

		void AddLogOutput(Log_Output_t LogOut){m_LogOutput |= LogOut;}
		void AddSpecialOutput(void *pToObj, pSpecifiedOutCallBack OutPutCallBack);
		void SetLogOutput(TU08_t LogOutPut){m_LogOutput = LogOutPut;}

		inline bool IsActive(){return m_LogActivated;}

		static CMyString<char> DateTimeNow();
private:
		void Initialise();
		bool SoundOnLog(Log_type_t LogType);
		void Log(const CMyString<char> & LogString);
		void LogDelayed(const CMyString<char> & LogString);
		bool LogToPrinter(const TS08_t  pComments);
		

		CFileInfo * m_pFileInfo;	///< Information describing the file

		bool m_LogActivated;	///< true if log activated
		TU08_t m_LogOutput;   ///< The log output
		bool m_DateAtLineBegin; ///< true to date at line begin
		CMyString<char> * m_pAppName;
		bool m_AppNameOnline;   ///< true to application name online

		pSpecifiedOutCallBack m_pOutCallBack;   ///< The out call back
		void * m_pToObj;	///< to object

		friend void LogThreadFunction(void *);
		CThread *m_pThreadLog;  ///< The thread log
		CMutex * m_pLogListAccessMutex; ///< The log list access mutex
		CMutex * m_pLogVarlistMutex;
		HANDLE m_hEventNewLogInList;	///< List of event new log ins
		CLinkedList<CMyString<char> *> m_LogList;	///< List of logs
};
#endif