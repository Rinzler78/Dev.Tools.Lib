#ifndef CTODOFILE_H
#define CTODOFILE_H
#include "Common\MyTypes.h"
#include "ConfigFile\ConfigFile.h"
#include "LinkedList\LinkedList.h"

class CLog;
class CIniSection;
class CIniSectionValue;

typedef enum ToDoActions
{
	ToDoActions_Copy,
	ToDoActions_Move,
	ToDoActions_Delete,
	ToDoActions_Launch,
	ToDoActions_InitValue,
	MAX_ToDoActions
}ToDoActions_t; ///< .
#define COPY_OR_MOVE_SEPARATOR '>'
const TC08_t ToDoActions_STR_TAB [MAX_ToDoActions][20] =  ///< .
{
	"Copy"
	, "Move"
	, "Delete"
	, "Launch"
};

class CToDoFile : public CConfigFile
{
public:
	CToDoFile(TS08_t PathAndFileName, bool DeleteActionAfterExecute = true, CLog * pLog = NULL);
	~CToDoFile();

	inline CIniSection * ToDoSection(){return m_pToDoListSection;};
	inline CIniSection * ParameterSection(){return m_pParameterSection;};
	inline CIniSectionValue * LogPathSectionValue(){return m_pLogpathSectionvalue;};
	bool PerformToDoList();
	bool CreateDefaultToDoFile();
	bool AddActionOnToDoList(ToDoActions_t Action, TS08_t ActionExplain);
private:

	/**********************************************************************************************//**
	 * \fn	void CToDoFile::Load();
	 *
	 * \brief	Loads this object.
	 *
	 * \author	Bleclere
	 * \date	12/03/2012
	 **************************************************************************************************/

	void Load();
	CLog * m_pLog;  ///< The log
	CIniSection * m_pToDoListSection;   ///< to do list section
	CIniSection * m_pParameterSection;  ///< The parameter section
	CIniSectionValue * m_pLogpathSectionvalue;  ///< The logpath sectionvalue
	bool m_DeleteActionAfterExecute;	///< true to delete the action after execute
};

#endif