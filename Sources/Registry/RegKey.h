#ifndef CREGKEY_H
#define CREGKEY_H
#include "windows.h"
#include "RegValue.h"
#include "LinkedList\LinkedList.h"
#define REGKEY_NAME_LENGTH 100

class CRegValue;

class CRegKey
{
/**********************************************************************************************//**
 * \fn	public;
 *
 * \brief	Initializes a new instance of the CRegKey class.
 *
 * \author	Bleclere
 * \date	23/02/2012
 *
 * \param	RegHKey	Handle of the register h key.
 * \param	pPath  	Full pathname of the file.
 **************************************************************************************************/

public : 
	CRegKey(HKEY RegHKey, const TS08_t  pPath);

	/**********************************************************************************************//**
	 * \fn	CRegKey::CRegKey(HKEY RegHKey, const TS16_t pPath);
	 *
	 * \brief	Initializes a new instance of the CRegKey class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	RegHKey	Handle of the register h key.
	 * \param	pPath  	Full pathname of the file.
	 **************************************************************************************************/

	CRegKey(HKEY RegHKey, const TS16_t  pPath);

	/**********************************************************************************************//**
	 * \fn	CRegKey::~CRegKey();
	 *
	 * \brief	Finalizes an instance of the CRegKey class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	~CRegKey();

	/**********************************************************************************************//**
	 * \fn	void CRegKey::Refresh();
	 *
	 * \brief	Refreshes this object.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	void Refresh();

	/**********************************************************************************************//**
	 * \fn	inline TS16_t CRegKey::Name()
	 *
	 * \brief	Gets the name.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	The name.
	 **************************************************************************************************/

	inline TS16_t Name(){return (TS16_t)&m_Name;};

	/**********************************************************************************************//**
	 * \fn	inline bool CRegKey::IsExist()
	 *
	 * \brief	Query if this object is exist.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	true if exist, false if not.
	 **************************************************************************************************/

	inline bool IsExist(){return m_Exist;};

	/**********************************************************************************************//**
	 * \fn	inline bool CRegKey::IsOpen()
	 *
	 * \brief	Query if this object is open.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	true if open, false if not.
	 **************************************************************************************************/

	inline bool IsOpen(){return m_Opened;};

	/**********************************************************************************************//**
	 * \fn	inline HKEY CRegKey::GetOpenenedKey()
	 *
	 * \brief	Gets the openened key.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	The openened key.
	 **************************************************************************************************/

	inline HKEY GetOpenenedKey(){return m_RegHKeyResult;}

	/**********************************************************************************************//**
	 * \fn	inline CLinkedList<CRegValue *> * CRegKey::GetValues()
	 *
	 * \brief	Gets the values.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	null if it fails, else the values.
	 **************************************************************************************************/

	inline CLinkedList<CRegValue *> * GetValues(){return &m_ValueList;};

	/**********************************************************************************************//**
	 * \fn	inline CLinkedList<CRegKey *> * CRegKey::GetKeys()
	 *
	 * \brief	Gets the keys.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	null if it fails, else the keys.
	 **************************************************************************************************/

	inline CLinkedList<CRegKey *> * GetKeys(){return &m_KeyList;};

	/**********************************************************************************************//**
	 * \fn	bool CRegKey::Delete();
	 *
	 * \brief	Deletes this object.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool Delete();

	/**********************************************************************************************//**
	 * \fn	bool CRegKey::DeleteKey(CRegKey *pRegKey);
	 *
	 * \brief	Deletes the key described by pRegKey.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param [in,out]	pRegKey	If non-null, the register key.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool DeleteKey(CRegKey *pRegKey);

	/**********************************************************************************************//**
	 * \fn	bool CRegKey::Delete(CRegValue *pRegValue);
	 *
	 * \brief	Deletes the given pRegValue.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param [in,out]	pRegValue	If non-null, the CRegValue * to delete.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool Delete(CRegValue *pRegValue);

	/**********************************************************************************************//**
	 * \fn	bool CRegKey::Open(TU32_t OpenAccess = KEY_READ);
	 *
	 * \brief	Opens.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	OpenAccess	(optional) the open access.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool Open(TU32_t OpenAccess = KEY_READ);

	/**********************************************************************************************//**
	 * \fn	bool CRegKey::Close();
	 *
	 * \brief	Closes this object.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool Close();

	/**********************************************************************************************//**
	 * \fn	bool CRegKey::Create(bool CloseAfterCreate = true);
	 *
	 * \brief	Creates this object.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	CloseAfterCreate	(optional) the close after create.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool Create(bool CloseAfterCreate = true);
private:
	CRegKey * m_pParentRegKey;  ///< The parent register key
	CLinkedList<CRegKey *> m_KeyList;   ///< List of keys
	CLinkedList<CRegValue *> m_ValueList;   ///< List of values
	HKEY m_RegHKey; ///< Handle of the register h key
	HKEY m_RegHKeyResult;   ///< Handle of the register h key result
	TC16_t m_Name [REGKEY_NAME_LENGTH];   ///< .
	TC16_t m_Path [REG_PATH_MAX_LENGTH];  ///< .
	bool m_Exist;   ///< true to exist
	bool m_Opened;  ///< true if opened
};
#endif