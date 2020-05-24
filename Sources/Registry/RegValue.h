#ifndef CREGVALUE_H
#define CREGVALUE_H

#define REG_PATH_MAX_LENGTH 250
#define REG_VALUE_NAME_MAX_LENGTH 250
#include "windows.h"
#include "Common\MyTypes.h"
#include "Registry\Regkey.h"

#define REG_VALUE_SIZE 200

/**********************************************************************************************//**
 * \enum	CRegValue_Type
 *
 * \brief	Values that represent CRegValue_Type.
 **************************************************************************************************/

typedef enum CRegValue_Type
{
	CRegValue_Type_Initvalue = 0,
	CRegValue_Type_String_Unicode,
	CRegValue_Type_String_ANSI,
	CRegValue_Type_32BITS_Number,
	CRegValue_Type_64BITS_Number,
	MAX_CRegValue_Type
}CRegValue_Type_t;

class CRegKey;

class CRegValue
{
/**********************************************************************************************//**
 * \fn	public;
 *
 * \brief	Initializes a new instance of the CRegValue class.
 *
 * \author	Bleclere
 * \date	23/02/2012
 *
 * \param [in,out]	pRegKey	If non-null, the register key.
 * \param	pValueName	   	Name of the value.
 **************************************************************************************************/

public : 
	CRegValue(CRegKey *pRegKey, const TS08_t pValueName);

	/**********************************************************************************************//**
	 * \fn	CRegValue::CRegValue(HKEY RegHKey, const TS08_t pPath, const TS08_t pValueName);
	 *
	 * \brief	Initializes a new instance of the CRegValue class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	RegHKey   	Handle of the register h key.
	 * \param	pPath	  	Full pathname of the file.
	 * \param	pValueName	Name of the value.
	 **************************************************************************************************/

	CRegValue(HKEY RegHKey, const TS08_t  pPath, const TS08_t pValueName);

	/**********************************************************************************************//**
	 * \fn	CRegValue::~CRegValue();
	 *
	 * \brief	Finalizes an instance of the CRegValue class.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	~CRegValue();

	/**********************************************************************************************//**
	 * \fn	void CRegValue::Refresh();
	 *
	 * \brief	Refreshes this object.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 **************************************************************************************************/

	void Refresh();

	/**********************************************************************************************//**
	 * \fn	inline TS16_t CRegValue::Name()
	 *
	 * \brief	Gets the name.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	The name.
	 **************************************************************************************************/

	inline TS16_t Name(){return (TS16_t)&m_ValueName;};

	/**********************************************************************************************//**
	 * \fn	inline bool CRegValue::IsExist()
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
	 * \fn	inline CRegValue_Type_t CRegValue::GetValueType()
	 *
	 * \brief	Gets the value type.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	The value type.
	 **************************************************************************************************/

	inline CRegValue_Type_t GetValueType(){return m_ValueType;};

	/**********************************************************************************************//**
	 * \fn	inline void * CRegValue::Value()
	 *
	 * \brief	Gets the value.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	null if it fails, else the value.
	 **************************************************************************************************/

	inline void * Value(){return m_pValue;};

	/**********************************************************************************************//**
	 * \fn	bool CRegValue::SetValue(void * pValue, TU32_t BytesCount, CRegValue_Type_t ValueType);
	 *
	 * \brief	Sets a value.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param [in,out]	pValue	If non-null, the value.
	 * \param	BytesCount	  	Number of bytes.
	 * \param	ValueType	  	Type of the value.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetValue(void * pValue, TU32_t BytesCount, CRegValue_Type_t ValueType);

	/**********************************************************************************************//**
	 * \fn	bool CRegValue::Delete();
	 *
	 * \brief	Deletes this object.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool Delete();
private:

	/**********************************************************************************************//**
	 * \fn	bool CRegValue::ClearValue();
	 *
	 * \brief	Clears the value.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool ClearValue();

	/**********************************************************************************************//**
	 * \fn	bool CRegValue::SetValue(BYTE * pValueFromRegistry, TU32_t BytesCount);
	 *
	 * \brief	Sets a value.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param [in,out]	pValueFromRegistry	If non-null, the value from registry.
	 * \param	BytesCount				  	Number of bytes.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetValue(BYTE * pValueFromRegistry, TU32_t BytesCount);

	/**********************************************************************************************//**
	 * \fn	bool CRegValue::RetrieveValue();
	 *
	 * \brief	Retrieves the value.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool RetrieveValue();
	TC16_t m_ValueName [REG_VALUE_NAME_MAX_LENGTH];   ///< .
	bool m_Exist;   ///< true to exist
	CRegValue_Type_t m_ValueType;   ///< Type of the value
	void * m_pValue;	///< The value
	CRegKey *m_pRegKey; ///< The register key
	bool m_RegObjectMustBeDestroy;  ///< true to register object must be destroy
};
#endif