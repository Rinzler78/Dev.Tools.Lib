#ifndef CMYSTRING_ADDSTRING_CPP
#define CMYSTRING_ADDSTRING_CPP
#include "MyString\MyString.h"
#include "String.h"
#include "stdlib.h"

#if defined (WIN32)
#include "Windows.h"
#else
#include <wchar.h>
#endif

#pragma region <Operators Add String>
TEMPLAT_TYPE_DECL
void CMyString<T>::operator += (const T * pString)
{
	AddString(StringLength(pString), pString);
}
TEMPLAT_TYPE_DECL
void CMyString<T>::operator += (T * pString)
{
	AddString(StringLength(pString), pString);
}
TEMPLAT_TYPE_DECL
void CMyString<T>::operator += (const CMyString<T> & SrcString)
{
	AddString(SrcString.Length(), SrcString.String());
}
template <typename T> template <typename ST>
void CMyString<T>::operator += (ST * pString)
{
	AddString(StringLength(pString), pString);
}
template <typename T> template <typename ST>
void CMyString<T>::operator += (const ST * pString)
{
	AddString(StringLength(pString), pString);
}
template <typename T> template <typename ST>
void CMyString<T>::operator += (const CMyString<ST> & SrcString)
{
	AddString(SrcString.Length(), SrcString.String());
}
TEMPLAT_TYPE_DECL
CMyString<T> CMyString<T>::operator + (const T * pString)
{
	return CMyString(pString);
}
TEMPLAT_TYPE_DECL
CMyString<T> CMyString<T>::operator + (const CMyString<T> & SrcString)
{
	return CMyString(SrcString);
}
template<typename T> template<typename ST> 
CMyString<T> CMyString<T>::operator + (const ST * pString)
{
	return CMyString(pString);
}
template<typename T> template<typename ST> 
CMyString<T> CMyString<T>::operator + (const CMyString<ST> & SrcString)
{
	return CMyString(SrcString);
}
#pragma endregion
#pragma region <Add String>
TEMPLAT_TYPE_DECL
void CMyString<T>::AddString(const T * pString)
{
	AddString((T *)pString);
}
TEMPLAT_TYPE_DECL
void CMyString<T>::AddString(T * pString)
{
	AddString(StringLength(pString), pString);
}
TEMPLAT_TYPE_DECL
void CMyString<T>::AddString(TU32_t Len, const T * pString)
{
	AddString(Len, (T *)pString);
}
TEMPLAT_TYPE_DECL
void CMyString<T>::AddString(TU32_t Len, T * pString)
{
	if(m_Length > 0)
	{
		/** Calculate Sizes */
		TU32_t NewSize = m_Length + Len;
		T * pNewString = NULL;

		if(NewSize > m_BufferLength)
		{
			/** Alloc and init New String */
			pNewString = new T[NewSize + 1];
			MACRO_MEMSET(pNewString, sizeof(T) * (NewSize + 1));
			memcpy(pNewString, m_pString, sizeof(T) * m_Length);
		}
		else
			pNewString = m_pString;

		memcpy(pNewString + m_Length, pString, sizeof(T) * Len);
		
		if(NewSize > m_BufferLength)
		{
			/** Set new String */
			MACRO_DELET_TAB(m_pString);
			m_pString = pNewString;
			m_BufferLength = NewSize;
		}
		m_Length = NewSize;
	}
	else
		SetString(Len, pString);

}
TEMPLAT_TYPE_DECL
void CMyString<T>::AddString(const T * pComments,...)
{
	va_list vl;
	
	va_start(vl, pComments);

	CMyString<T> VarList = GetVarList(vl, pComments);

	va_end(vl);
	
	AddString(VarList.Length(), VarList.String());
}

template<typename T> template<typename ST>
void CMyString<T>::AddString(ST * pString)
{
	AddString(StringLength(pString), pString);
}

template<typename T> template<typename ST>
void CMyString<T>::AddString(const ST * pString)
{
	AddString(StringLength(pString), pString);
}

/*****/
template<typename T> template<typename ST>
inline void CMyString<T>::AddString(TU32_t Len, const ST * pString)
{
	AddString(Len, (ST *) pString);
}

/****/
template<typename T> template<typename ST>
inline void CMyString<T>::AddString(TU32_t Len, ST * pString)
{
	printf("Undefined Specialisation : CMyString<%s>::AddString(TU32_t Len, %s * pString)", typeid(T).name(), typeid(ST).name());
}
template<> template<>
inline void CMyString<char>::AddString(TU32_t Len, wchar_t * pString)
{
	/** Calculate Sizes */
	if(!pString || (Len == 0))
		return;
	
	TU32_t NewSize = m_Length + Len;
	
	/** Alloc and init New String */
	char * pNewString = new char[NewSize + 1];
	MACRO_MEMSET(pNewString, (NewSize + 1));
	
	/** Construct New String */
	if(m_pString)
		memcpy(pNewString, m_pString, m_Length);
	
	wcstombs((char *)pNewString + m_Length, (wchar_t *)pString, Len);
	
	/** Set new String */
	SetString(NewSize, pNewString);
	MACRO_DELET_TAB(pNewString);
}
template<> template<>
inline void CMyString<wchar_t>::AddString(TU32_t Len, char * pString)
{
	/** Calculate Sizes */
	if(!pString || (Len == 0))
		return;

	TU32_t NewSize = m_Length + Len;
	
	/** Alloc and init New String */
	wchar_t * pNewString = new wchar_t[NewSize + 1];
	MACRO_MEMSET(pNewString, 2 * (NewSize + 1));
	
	/** Construct New String */
	if(m_pString)
		memcpy(pNewString, m_pString, 2 * m_Length);
	
	mbstowcs((wchar_t *)pNewString + m_Length, (char *)pString, Len);
	
	/** Set new String */
	SetString(NewSize, pNewString);
	MACRO_DELET_TAB(pNewString);
}

#pragma endregion
#endif