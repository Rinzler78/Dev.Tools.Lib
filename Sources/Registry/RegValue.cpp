#ifndef CREGVALUE_CPP
#define CREGVALUE_CPP
#include "RegValue.h"
const TU32_t REG_VALUE_TYPE_TEB[MAX_CRegValue_Type]\
={0,REG_SZ,REG_SZ, REG_DWORD, REG_QWORD};

CRegValue::CRegValue(CRegKey *pRegKey, const TS08_t  pValueName)
: m_pRegKey(pRegKey)
, m_Exist(false)
, m_ValueType(CRegValue_Type_Initvalue)
, m_pValue(NULL)
, m_RegObjectMustBeDestroy(false)

{
	/** Memory init */
	MACRO_MEMSET(m_ValueName, sizeof(m_ValueName))

	if(pValueName)
		mbstowcs(m_ValueName,pValueName,sizeof(m_ValueName));
}
CRegValue::CRegValue(HKEY RegHKey, const TS08_t  pPath, const TS08_t pValueName)
: m_Exist(false)
, m_ValueType(CRegValue_Type_Initvalue)
, m_pValue(NULL)
, m_RegObjectMustBeDestroy(true)
{
	/** Memory init */
	MACRO_MEMSET(m_ValueName, sizeof(m_ValueName))

	if(pValueName)
		mbstowcs(m_ValueName,pValueName,sizeof(m_ValueName));
	
	m_pRegKey = new CRegKey(RegHKey, pPath);
	m_pRegKey->Refresh();
	Refresh();
}
CRegValue::~CRegValue()
{
	/** Free Memory */
	ClearValue();
	/** Reg Key Object*/
	if(m_RegObjectMustBeDestroy)
		MACRO_DELET(m_pRegKey);
}
void CRegValue::Refresh()
{
	/** Check value Existing */
	RetrieveValue();
}
bool CRegValue::RetrieveValue()
{
	bool Result = false;
	DWORD Count = 0;
	DWORD DataType = 0;
	TU08_t VALUE [REG_VALUE_SIZE] = {0};
	Count = REG_VALUE_SIZE;

	if(m_pRegKey)
	{
		if(!m_pRegKey->IsOpen())
			m_pRegKey->Open();
		/** Take Value */
		if(m_pRegKey->IsOpen())
		{
			Result = (RegQueryValueEx(m_pRegKey->GetOpenenedKey(),m_ValueName,NULL,&DataType,(LPBYTE)VALUE, &Count) == ERROR_SUCCESS);
			m_Exist = Result;
			/** Close Key */
			m_pRegKey->Close();
			switch(DataType)
			{
				case REG_SZ:
					if((strlen((TS08_t)VALUE) + 1) < Count)
						m_ValueType = CRegValue_Type_String_Unicode;
					else
						m_ValueType = CRegValue_Type_String_ANSI;
					break;
				case REG_DWORD:
				case REG_DWORD_BIG_ENDIAN:
					m_ValueType = CRegValue_Type_32BITS_Number;
					break;
				case REG_QWORD:
					m_ValueType = CRegValue_Type_64BITS_Number;
					break;
			}
			SetValue(VALUE, Count);
		}
	}

	return Result;
}
bool CRegValue::ClearValue()
{
	bool Result = false;
	
	if(m_pValue)
	{
		Result = true;
		switch(m_ValueType)
		{
		case CRegValue_Type_String_Unicode:
		case CRegValue_Type_String_ANSI:
			MACRO_DELET_TAB(m_pValue);
			break;
		case CRegValue_Type_32BITS_Number:
		case CRegValue_Type_64BITS_Number:
			MACRO_DELET(m_pValue);
			break;
		default:
			Result = false;
			break;
		}
	}

	return Result;
}
bool CRegValue::SetValue(BYTE * pValueFromRegistry, TU32_t BytesCount)
{
	bool Result = false;
	
	if(m_pValue)
		ClearValue();
	if(pValueFromRegistry && !m_pValue && (BytesCount > 0))
	{
		Result = true;
		switch(m_ValueType)
		{
		case CRegValue_Type_String_Unicode:
			m_pValue = new TC16_t[BytesCount];
			MACRO_MEMSET(m_pValue, BytesCount * 2)
			memcpy(m_pValue,pValueFromRegistry,BytesCount * 2);
			break;
		case CRegValue_Type_String_ANSI:
			m_pValue = new TC08_t[BytesCount];
			MACRO_MEMSET(m_pValue, BytesCount)
			memcpy(m_pValue,pValueFromRegistry,BytesCount);
			break;
		case CRegValue_Type_32BITS_Number:
			if(BytesCount == 4)
			{
				TU32_t *pV = new TU32_t; 
				
				//*pV =  (((TU32_t)pValueFromRegistry[0]) << 24
				//				| ((TU32_t)pValueFromRegistry[1]) << 16
				//				| ((TU32_t)pValueFromRegistry[2]) << 8
				//				| ((TU32_t)pValueFromRegistry[3]));

				/** NOTE : Buffer returned by resgistry is filled from left to right */
				*pV =  (((TU32_t)pValueFromRegistry[3]) << 24
								| ((TU32_t)pValueFromRegistry[2]) << 16
								| ((TU32_t)pValueFromRegistry[1]) << 8
								| ((TU32_t)pValueFromRegistry[0]));
				m_pValue = (void *)pV;
			}
			break;
		case CRegValue_Type_64BITS_Number:
			if(BytesCount == 8)
			{
				m_pValue = new TU64_t;
				/*m_pValue = (void *)(((TU64_t	)pValueFromRegistry[0]) << 56
									| ((TU64_t )pValueFromRegistry[1]) << 48
									| ((TU64_t )pValueFromRegistry[2]) << 40
									| ((TU64_t )pValueFromRegistry[3]) << 32
									| ((TU64_t )pValueFromRegistry[4]) << 24
									| ((TU64_t )pValueFromRegistry[5]) << 16
									| ((TU64_t )pValueFromRegistry[6]) << 8
									| ((TU64_t )pValueFromRegistry[7]));*/

				/** NOTE : Buffer returned by resgistry is filled from left to right */
				m_pValue = (void *)(((TU64_t	)pValueFromRegistry[7]) << 56
									| ((TU64_t )pValueFromRegistry[6]) << 48
									| ((TU64_t )pValueFromRegistry[5]) << 40
									| ((TU64_t )pValueFromRegistry[4]) << 32
									| ((TU64_t )pValueFromRegistry[3]) << 24
									| ((TU64_t )pValueFromRegistry[2]) << 16
									| ((TU64_t )pValueFromRegistry[1]) << 8
									| ((TU64_t )pValueFromRegistry[0]));
			}
			break;
		default:
			Result = false;
			break;
		}
	}

	return Result;
}
bool CRegValue::SetValue(void * pValue, TU32_t BytesCount, CRegValue_Type_t ValueType)
{
	bool Result = false;
	TU32_t Size = 0;
	
	if(m_pRegKey && pValue && (BytesCount > 0))
	{
		//if(m_ValueType == CRegValue_Type_Initvalue)
		//	m_ValueType = ValueType;
		
		//if(ValueType == m_ValueType)
		//{
		if(!m_pRegKey->IsExist())
			m_pRegKey->Create(false);
		else
			m_pRegKey->Open(KEY_SET_VALUE);

		if(m_pRegKey->IsOpen())
		{
			Result = (RegSetValueEx(m_pRegKey->GetOpenenedKey(),m_ValueName,NULL,REG_VALUE_TYPE_TEB[ValueType],(BYTE *)pValue,BytesCount) == ERROR_SUCCESS);
			m_Exist = Result;
			m_ValueType = ValueType;
			m_pRegKey->Close();
		}
	}

	return Result;
}
bool CRegValue::Delete()
{
	bool Result = false;
	
	if(m_pRegKey)
	{
		m_pRegKey->Open(KEY_SET_VALUE);

		if(m_pRegKey->IsOpen())
		{
			Result = (RegDeleteValue(m_pRegKey->GetOpenenedKey(),m_ValueName) == ERROR_SUCCESS);
			m_Exist = !Result;
			m_ValueType = CRegValue_Type_Initvalue;
			m_pRegKey->Close();
		}
	}
	return Result;
}
#endif