#ifndef CMYSTRING_STRINGLENGTH_CPP
#define CMYSTRING_STRINGLENGTH_CPP
#include "MyString\MyString.h"
#include "String.h"
#include "stdlib.h"

#if defined (WIN32)
#include "Windows.h"
#else
#include <wchar.h>
#endif

#pragma region <String Length>
template <typename T> template <typename ST>
inline TU32_t CMyString<T>::StringLength(ST * pString)
{
	return 0;
}
template<> template<>//String Length  Specialisation
inline TU32_t CMyString<char>::StringLength(char * pString)
{
	return (pString ? (TU32_t)strlen(pString) : 0);
}
template<> template<>//String Length  Specialisation
inline TU32_t CMyString<char>::StringLength(const char * pString)
{
	return (pString ? (TU32_t)strlen(pString) : 0);
}
template<> template<>//String Length  Specialisation
inline TU32_t CMyString<char>::StringLength(wchar_t * pString)
{
	return (pString ? (TU32_t)wcslen(pString) : 0);
}
template<> template<>//String Length  Specialisation
inline TU32_t CMyString<char>::StringLength(const wchar_t * pString)
{
	return (pString ? (TU32_t)wcslen(pString) : 0);
}
/** T = wchar */
template<> template<>//String Length  Specialisation
inline TU32_t CMyString<wchar_t>::StringLength(char * pString)
{
	return (pString ? (TU32_t)strlen(pString) : 0);
}
template<> template<>//String Length  Specialisation
inline TU32_t CMyString<wchar_t>::StringLength(const char * pString)
{
	return (pString ? (TU32_t)strlen(pString) : 0);
}
template<> template<>//String Length  Specialisation
inline TU32_t CMyString<wchar_t>::StringLength(wchar_t * pString)
{
	return (pString ? (TU32_t)wcslen(pString) : 0);
}
template<> template<>//String Length  Specialisation
inline TU32_t CMyString<wchar_t>::StringLength(const wchar_t * pString)
{
	return (pString ? (TU32_t)wcslen(pString) : 0);
}

#pragma endregion
#endif