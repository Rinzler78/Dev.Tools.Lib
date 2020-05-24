#ifndef CMYSTRING_SETSTRING_CPP
#define CMYSTRING_SETSTRING_CPP
#include "MyString\MyString.h"
#include "String.h"
#include "stdlib.h"

#if defined (WIN32)
#include "Windows.h"
#else
#include <wchar.h>
#endif
#pragma region <Set String>
TEMPLAT_TYPE_DECL
inline void CMyString<T>::SetString(T * pString)
{
	SetString(StringLength(pString), pString);
}
TEMPLAT_TYPE_DECL
inline void CMyString<T>::SetString(const T * pComments,...)
{
	va_list vl;

	va_start(vl, pComments);
	
	(*this) = GetVarList(vl, pComments);

	va_end(vl);
}
TEMPLAT_TYPE_DECL
inline void CMyString<T>::SetString(TU32_t Size, const T * pString)
{
	SetString(Size, (T *)pString);
}
TEMPLAT_TYPE_DECL
inline void CMyString<T>::SetString(TU32_t Size, T * pString)
{
	if(pString)
	{
		if(Size > m_BufferLength)
		{
			MACRO_DELET_TAB(m_pString)
			m_BufferLength = Size;
		}
		
		m_Length = Size;

		if(!m_pString)
			m_pString = new T[m_BufferLength + 1];
		
		MACRO_MEMSET(m_pString, sizeof(T) * (m_BufferLength + 1));
		
		/** Set new String */
		memcpy(m_pString, pString,  sizeof(T) * m_Length);
	}
}
template<typename T> template<typename ST>
inline void CMyString<T>::SetString(TU32_t Size, ST * pString)
{
}
template<> template<>
inline void CMyString<char>::SetString(TU32_t Size, wchar_t * pString)
{
#if defined (WIN32)
	TU32_t NbCharConverted = 0;
#endif
	if(pString)
	{
		if(Size > m_BufferLength)
		{
			MACRO_DELET_TAB(m_pString)
			m_BufferLength = Size;
		}
		
		m_Length = Size;
		
		/** Alloc And Init New String */
		if(!m_pString)
			m_pString = new char[m_Length + 1];

		MACRO_MEMSET(m_pString, sizeof(char) * (m_Length + 1));

		/** Set new String */
#if defined (WIN32)
		wcstombs_s(	&NbCharConverted
					, m_pString
					, m_Length + 1
					, pString
					, sizeof(wchar_t) * (m_Length + 1));
#else
		wcstombs(m_pString, pString, m_Length + 1);
#endif
	}
}
template<> template<>
inline void CMyString<wchar_t>::SetString(TU32_t Size, char * pString)
{
#if defined (WIN32)
	TU32_t NbCharConverted = 0;
#endif

	if(pString)
	{
		if(Size > m_BufferLength)
		{
			MACRO_DELET_TAB(m_pString)
			m_BufferLength = Size;
		}
		
		m_Length = Size;
		
		/** Alloc And Init New String */
		if(!m_pString)
			m_pString = new wchar_t[m_Length + 1];

		MACRO_MEMSET(m_pString, sizeof(wchar_t) * (m_Length + 1));

		/** Set new String */
#if defined (WIN32)
		mbstowcs_s(	&NbCharConverted
					, m_pString
					, (m_Length + 1)
					, pString
					, (m_Length));
		
#else
		mbstowcs(m_pString, pString, m_Length + 1);
#endif
	}
}
template<typename T> template<typename ST>
void CMyString<T>::SetString(TU32_t Size, const ST * pString)
{
	SetString(Size, (ST *)pString);
}

template<typename T> template<typename ST>
inline void CMyString<T>::SetString(ST * pString)
{
	SetString(StringLength(pString), pString);
}
#pragma region <Operators Set String>
TEMPLAT_TYPE_DECL
inline void CMyString<T>::operator = (const T * pString)
{
	SetString(StringLength(pString), pString);
}
TEMPLAT_TYPE_DECL
inline void CMyString<T>::operator = (T * pString)
{
	SetString(StringLength(pString), pString);
}
TEMPLAT_TYPE_DECL
inline void CMyString<T>::operator = (const CMyString<T> & SrcString)
{
	SetString(SrcString.Length(), SrcString.String());
}
template<typename T> template<typename ST> 
inline void CMyString<T>::operator = (const ST * pString)
{
	SetString(StringLength(pString), pString);
}
template<typename T> template<typename ST> 
inline void CMyString<T>::operator = (ST * pString)
{
	SetString(StringLength(pString), pString);
}
template<typename T> template<typename ST> 
inline void CMyString<T>::operator = (const CMyString<ST> & SrcString)
{
	SetString(SrcString.Length(), SrcString.String());
}
#pragma endregion

#pragma endregion 
#endif