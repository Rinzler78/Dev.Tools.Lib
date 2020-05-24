#ifndef CMYSTRING_STRINGTESTS_CPP
#define CMYSTRING_STRINGTESTS_CPP
#include "MyString\MyString.h"
#include "String.h"
#include "stdlib.h"

#if defined (WIN32)
#include "Windows.h"
#else
#include <wchar.h>
#endif
#pragma region <Is Equal>
template<typename T> template<typename ST>
bool CMyString<T>::IsEqual(const ST * pString)const
{
	return IsEqual(StringLength(pString), pString);
}
template<typename T> template<typename ST>
bool CMyString<T>::IsEqual(TU32_t Len, const ST * pString)const
{
	if(!pString)
		return false;
	
	CMyString<T> Temp;
	Temp.AddString<>(Len, pString);

	return IsEqual(Temp);
}
template<typename T> template<typename ST>
bool CMyString<T>::IsEqual(const CMyString<ST> & String)const
{
	return IsEqual(String.Length(), String.String());
}
template<typename T>
bool CMyString<T>::IsEqual(const T * pString)const
{
	return IsEqual((T *)pString, StringLength(pString));
}
template<typename T>
bool CMyString<T>::IsEqual(TU32_t Len, const T * pString)const
{
	if(pString && (m_Length == Len))
		return (memcmp(m_pString, pString, sizeof(T) * m_Length ) == 0);
	
	return false;
}
template<typename T>
bool CMyString<T>::IsEqual(const CMyString<T> & String)const
{
	return IsEqual(String.Length(), String.String());
}

#pragma endregion
#pragma region <Operators Equality>
template<typename T> template<typename ST> 
bool CMyString<T>::operator == (const CMyString<ST> & SrcString)const
{
	return IsEqual(SrcString.Length(), SrcString.String());
}
template<typename T> template<typename ST> 
bool CMyString<T>::operator == (const ST * pString)const
{
	return IsEqual(StringLength(pString), pString);
}
template<typename T> template<typename ST> 
bool CMyString<T>::operator == (ST * pString)const
{
	return IsEqual(StringLength(pString), pString);
}

template<typename T> template<typename ST> 
bool CMyString<T>::operator != (const CMyString<ST> & SrcString)const
{
	return !IsEqual(SrcString.Length(), SrcString.String());
}
//template<typename T> template<typename ST> 
//bool CMyString<T>::operator != (const ST * pString)const
//{
//	return !IsEqual(StringLength(pString), pString);
//}
template<typename T> template<typename ST> 
bool CMyString<T>::operator != (ST * pString)const
{
	return !IsEqual(StringLength(pString), pString);
}

TEMPLAT_TYPE_DECL
bool CMyString<T>::operator == (const CMyString<T> & SrcString)const
{
	return IsEqual(SrcString.Length(), SrcString.String());
}
TEMPLAT_TYPE_DECL
bool CMyString<T>::operator == (const T * pString)const
{
	return IsEqual(StringLength(pString), pString);
}
TEMPLAT_TYPE_DECL
bool CMyString<T>::operator == (T * pString)const
{
	return IsEqual(StringLength(pString), pString);
}

TEMPLAT_TYPE_DECL
bool CMyString<T>::operator != (const CMyString<T> & SrcString)const
{
	return !IsEqual(SrcString.Length(), SrcString.String());
}
TEMPLAT_TYPE_DECL
bool CMyString<T>::operator != (const T * pString)const
{
	return !IsEqual(StringLength(pString), pString);
}
TEMPLAT_TYPE_DECL
bool CMyString<T>::operator != (T * pString)const
{
	return !IsEqual(StringLength(pString), pString);
}

#pragma endregion
#endif