#ifndef CMYSTRING_CPP
#define CMYSTRING_CPP
#include "LinkedList\LinkedList.h"
#include "MyString\MyString.h"
#include "String.h"
#include "stdlib.h"

#if defined (WIN32) || defined (WINCE)
#include "Windows.h"
#else
#include <wchar.h>
#endif

#pragma region <Constructor>
TEMPLAT_TYPE_DECL
CMyString<T>::CMyString()
:m_pString(NULL)
, m_Length(0)
, m_BufferLength(0)
{
}
TEMPLAT_TYPE_DECL
CMyString<T>::CMyString(T * pString)
:m_pString(NULL)
, m_Length(0)
, m_BufferLength(0)
{
	SetString(StringLength(pString), pString);
}
TEMPLAT_TYPE_DECL
CMyString<T>::CMyString(const T * pString)
:m_pString(NULL)
, m_Length(0)
, m_BufferLength(0)
{
	SetString(StringLength(pString), pString);
}
TEMPLAT_TYPE_DECL
CMyString<T>::CMyString(TU32_t Size, T * pString)
:m_pString(NULL)
, m_Length(0)
, m_BufferLength(0)
{
	SetString(Size, pString);
}
TEMPLAT_TYPE_DECL
CMyString<T>::CMyString(TU32_t Size, const T * pString)
:m_pString(NULL)
, m_Length(0)
, m_BufferLength(0)
{
	SetString(Size, pString);
}
TEMPLAT_TYPE_DECL
CMyString<T>::CMyString(const CMyString<T> & Src)
:m_pString(NULL)
, m_Length(0)
, m_BufferLength(0)
{
	SetString(Src.Length(), Src.String());
}
TEMPLAT_TYPE_DECL
CMyString<T>::CMyString(CMyString<T> * pSrc)
:m_pString(NULL)
, m_Length(0)
, m_BufferLength(0)
{
	if(pSrc)
		SetString(pSrc->Length(), pSrc->String());
}
template<typename T> template<typename ST> 
CMyString<T>::CMyString(ST * pString)
:m_pString(NULL)
, m_Length(0)
, m_BufferLength(0)
{
	SetString(StringLength(pString), pString);
}
template<typename T> template<typename ST> 
CMyString<T>::CMyString(const ST * pString)
:m_pString(NULL)
, m_Length(0)
, m_BufferLength(0)
{
	SetString(StringLength(pString), pString);
}
template<typename T> template<typename ST> 
CMyString<T>::CMyString(TU32_t Size, ST * pString)
:m_pString(NULL)
, m_Length(0)
, m_BufferLength(0)
{
	SetString(Size, pString);
}
template<typename T> template<typename ST> 
CMyString<T>::CMyString(TU32_t Size, const ST * pString)
:m_pString(NULL)
, m_Length(0)
, m_BufferLength(0)
{
	SetString(Size, pString);
}
template<typename T> template<typename ST> 
CMyString<T>::CMyString(const CMyString<ST> & Src)
:m_pString(NULL)
, m_Length(0)
, m_BufferLength(0)
{
	SetString(Src.Length(), Src.String());
}
template<typename T> template<typename ST> 
CMyString<T>::CMyString(CMyString<ST> * pSrc)
:m_pString(NULL)
, m_Length(0)
, m_BufferLength(0)
{
	if(pSrc)
		SetString(pSrc->Length(), pSrc->String());
}
#pragma endregion
TEMPLAT_TYPE_DECL
CMyString<T>::~CMyString(void)
{
#if defined (_DEBUG)
	//printf("Delete String Type : %s\nValue : %s\n", typeid(T).name(), m_pString);
#endif
	MACRO_DELET_TAB(m_pString)
}

#pragma region <Split String>
TEMPLAT_TYPE_DECL
CLinkedList<CMyString<T> *> * CMyString<T>::Split(T Separator) const
{
	CLinkedList<CMyString<T> *> * pResult = NULL;
	T * pMobilePtr = String();
	T * pStringFound = String();
	TU32_t WordSize = 0;
	
	while(*pMobilePtr)
	{
		if(*pMobilePtr == Separator)
		{
			if(!pResult)
				pResult = new CLinkedList<CMyString<T> *>();
			
			CMyString<T> * pString = new CMyString<T>(WordSize, pStringFound);
			pResult->Add(pString);

			pStringFound = pMobilePtr + 1;
			WordSize = 0;
			++pMobilePtr;
			continue;
		}
		++pMobilePtr;
		++WordSize;
	}
	return pResult;
}
TEMPLAT_TYPE_DECL
CLinkedList<CMyString<T> *> * CMyString<T>::Split(T * pSeparatorString, TU32_t SeparatorStringSize)const
{
	CLinkedList<CMyString<T> *> * pResult = NULL;
	TU32_t Index = 0;
	T * pToString = String();
	T * pStringFound = String();
	TU32_t WordSize = 0;
	
	if(pSeparatorString && (SeparatorStringSize > 0))
	{
		if(SeparatorStringSize > 1)
		{
			while(Index <= (Length() - SeparatorStringSize))
			{
				if(	(pToString[Index] == pSeparatorString[0])
					&& (memcmp(&pToString[Index + 1], &pSeparatorString[1], sizeof(T) * (SeparatorStringSize - 1)) == 0))
				{
					if(!pResult)
						pResult = new CLinkedList<CMyString<T> *>();
					
					CMyString<T> * pString = new CMyString<T>(WordSize, pStringFound);
					pResult->Add(pString);

					Index += SeparatorStringSize;
					pStringFound = &pToString[Index];
					WordSize = 0;
					continue;
				}
				++Index;
				++WordSize;
			}
			
			if(pStringFound)
			{
				TU32_t StrLength = StringLength(pStringFound);
				if(StrLength)
				{
					/** Add remaining */
					CMyString<T> * pString = new CMyString<T>(StrLength, pStringFound);
					pResult->Add(pString);
				}
			}
		}
		else 
			pResult = Split(pSeparatorString[0]);

	}
	return pResult;
}
TEMPLAT_TYPE_DECL
void CMyString<T>::Split(T Separator, CMyStringDelegate<T> & Delegate)const
{
	T * pMobilePtr = String();
	T * pStringFound = String();
	TU32_t WordSize = 0;
	
	while(*pMobilePtr)
	{
		if(*pMobilePtr == Separator)
		{
			Delegate.FieldFound(CMyString<T>(pStringFound, WordSize));

			pStringFound = pMobilePtr + 1;
			WordSize = 0;
			++pMobilePtr;
			continue;
		}
		++pMobilePtr;
		++WordSize;
	}
}
TEMPLAT_TYPE_DECL
void CMyString<T>::Split(T * pSeparatorString, TU32_t SeparatorStringSize, CMyStringDelegate<T> & Delegate)const
{
	TU32_t Index = 0;
	T * pToString = String();
	T * pStringFound = String();
	TU32_t WordSize = 0;
	TU32_t WorCount = 0;
	
	if(pSeparatorString && (SeparatorStringSize > 0))
	{
		if(SeparatorStringSize > 1)
		{
			while(Index <= (Length() - SeparatorStringSize))
			{
				if(	(pToString[Index] == pSeparatorString[0])
					&& (memcmp(&pToString[Index + 1], &pSeparatorString[1], sizeof(T) * (SeparatorStringSize - 1)) == 0))
				{
					++WorCount;
					Delegate.FieldFound(WorCount, Index, CMyString<T>(pStringFound, WordSize));

					Index += SeparatorStringSize;
					pStringFound = &pToString[Index];
					WordSize = 0;
					continue;
				}
				++Index;
				++WordSize;
			}
		}
		else 
			Split(pSeparatorString[0], Delegate);

	}
}
#pragma endregion
#pragma region <Find In String>
TEMPLAT_TYPE_DECL
TI32_t CMyString<T>::FindFirst(const T * pString, const TU32_t StringLen, const T CharSearch)
{
	if(!pString || !StringLen) return 0;
	
	TI32_t Result = 0;

	while((pString[Result] != NULL) && (pString[Result] != CharSearch))
		++Result;

	if(!pString[Result])
		Result = -1;

	return Result;
}
TEMPLAT_TYPE_DECL
TI32_t CMyString<T>::FindFirst(const T Seperator)const
{
	TI32_t Result = -1;
	TU32_t Index = 0;
	T * pString = String();

	while((Index < Length()) && (pString[Index] != Seperator))
		++Index;

	if(Index < Length())
		Result = Index;

	return Result;
}
TEMPLAT_TYPE_DECL
TI32_t CMyString<T>::FindLast(const T * pString, const TU32_t StringLen, const T CharSearch)
{
	if(!pString || !StringLen) return 0;
	
	TI32_t Result = StringLen - 1;

	while((Result > -1) && (pString[Result] != CharSearch))
		--Result;

	if(pString[Result] != CharSearch)
		Result = -1;

	return Result;
}
TEMPLAT_TYPE_DECL
TI32_t CMyString<T>::FindLast(const T Seperator)const
{
	TI32_t Result = -1;
	TI32_t Index = 0;
	T * pString = String();

	Index = Length() - 1;
	while((Index >= 0) && (pString[Index] != Seperator))
		--Index;

	if(Index >= 0)
		Result = Index;

	return Result;
}
TEMPLAT_TYPE_DECL
TI32_t CMyString<T>::FindFirst(const T * pStringSearch, TU32_t StringSearchSize)const
{
	TI32_t Result = -1;
	TI32_t Index = 0;
	T * pString = String();

	if(pStringSearch && (StringSearchSize > 0))
	{
		if(StringSearchSize > 1)
		{
			while(Index < (Length() - StringSearchSize))
			{
				if(	(pString[Index] == pStringSearch[0])
					&& (memcmp(&pString[Index + 1], &pStringSearch[1], sizeof(T) * (StringSearchSize - 1)) == 0))
				{
					Result = Index;
					break;
				}
				++Index;
			}
		}
		else
			Result = FindFirst(pStringSearch[0]);
	}

	return Result;
}
TEMPLAT_TYPE_DECL
TI32_t CMyString<T>::FindLast(const T * pStringSearch, TU32_t StringSearchSize)const
{
	TI32_t Result = -1;
	TI32_t Index = 0;
	T * pString = String();

	if(pStringSearch && (StringSearchSize > 0))
	{
		if(StringSearchSize > 1)
		{
			Index = (Length() - StringSearchSize);
			while(Index >= 0)
			{
				if(	(pString[Index] == pStringSearch[0])
					&& (memcmp(&pString[Index + 1], &pStringSearch[1], sizeof(T) * (StringSearchSize - 1)) == 0))
				{
					Result = Index;
					break;
				}
				--Index;
			}
		}
		else
			Result = FindLast(pStringSearch[0]);
	}

	return Result;
}
#pragma endregion
TEMPLAT_TYPE_DECL
bool CMyString<T>::SupressBefore(T Char)
{
	bool Result = false;
	TI32_t Index = -1;
	
	if(m_pString)
	{
		while(m_pString[Index + 1] == Char)
			++Index;
		
		if((Index > -1) && (Index < (TI32_t)m_Length))
		{
			++Index;
			memcpy(m_pString, m_pString + Index, (m_Length - Index) * sizeof(T));
			m_Length -= Index;
			m_pString [m_Length] = NULL;
		}
		Result = true;
	}

	return Result;
}
TEMPLAT_TYPE_DECL
bool CMyString<T>::SupressBefore(T * String)
{
	if(!String)
		return false;

	bool Result = false;
	TU32_t STRLength = StringLength(String);
	TI32_t Index = -1;
	
	if(STRLength > 0)
	{
		if(STRLength == 1)
			Result = SupressBefore(String[0]);
		else
		{
			while(!memcmp(this->String() + (Index + 1), String, STRLength))
				Index += STRLength;
			
			if(Index > -1)
			{
				memcpy(m_pString, m_pString + Index + 1, (m_Length - (Index + 1)) * sizeof(T));
				m_Length -= (Index + 1);
				m_pString [m_Length] = NULL;
			}
		}
	}
	
	return Result;
}
TEMPLAT_TYPE_DECL
bool CMyString<T>::SupressBefore(const T * String)
{
	return SupressBefore((T *)String);
}
TEMPLAT_TYPE_DECL
bool CMyString<T>::SupressAfter(T Char)
{
	bool Result = false;
	
	if(m_pString)
	{
		while((m_pString[m_Length - 1] == Char) && (m_Length > 0))
			--m_Length;

		m_pString[m_Length] = NULL;

		Result = true;
	}

	return Result;
}
TEMPLAT_TYPE_DECL
bool CMyString<T>::SupressAfter(T * pString)
{
	if(!String)
		return false;

	bool Result = false;
	TU32_t STRLength = StringLength(pString);
	TI32_t Index = 0;
	T * pThisString = NULL;
	
	if(STRLength > 0)
	{
		if(STRLength == 1)
			SupressAfter(String[0]);			
		else
		{
			Index = this->Length() - 1;

			do
			{
				Index -= (STRLength);
				pThisString = this->String() + Index;
			}
			while(!memcmp(pThisString, pString, sizeof(T) * STRLength));

			if(Index > -1)
			{
				m_pString[Index] = NULL;
				m_Length = Index;
			}
		}
	}
	
	return Result;
}
TEMPLAT_TYPE_DECL
bool CMyString<T>::SupressAfter(const T * String)
{
	return SupressAfter((T *)String);
}
TEMPLAT_TYPE_DECL
bool CMyString<T>::Replace(T CurrentChar, T NewChar)
{
	bool Result = false;
	
	for(TU32_t i = 0 ; i < m_Length ; ++i)
		if(m_pString[i] == CurrentChar)
			m_pString[i] = NewChar;

	return Result;
}
TEMPLAT_TYPE_DECL
CMyString<T> CMyString<T>::GetString(const T * pPattern, ...)
{
	va_list vl;
	va_start(vl, pPattern);
	CMyString<T> Result = GetVarList(vl, pPattern);
	va_end(vl);

	return Result; 
}
TEMPLAT_TYPE_DECL
CMyString<T> CMyString<T>::GetVarList(va_list vl, const T * pComments,...)
{
	return CMyString<T>();
}
template <>
CMyString<char> CMyString<char>::GetVarList(va_list vl, const char * pComments,...)
{
	TU32_t i = 0;
	TU32_t StringStartPoint = 0;
	CMyString<char> Temp;

	const TU32_t VarListSTRMaxSize = 500;
	TC08_t TempChar [VarListSTRMaxSize] = {0};

	if(pComments)
	{
		if(vl)
		{
			// Step through the list.
			for( i = 0; pComments[i] != '\0'; ++i ) 
			{
				if(pComments[i] != '%')
					continue;
				
				if(i > StringStartPoint)
					Temp.AddString((i - StringStartPoint), pComments + StringStartPoint);

				++i;

				/** Check value type %. */
				switch( pComments[i] ) 
				{   
					case 'c':
						/** Concat TempString and value */
						sprintf(TempChar
								,"%c"
								,va_arg( vl, TC08_t ));
					break;
					case 'i':
					case 'd':
						/** Concat TempString and value */
						sprintf(TempChar
								,"%i"
								,va_arg( vl, TI32_t ));
					break;
					case 'u':
						if(pComments[i + 1] == 'l')
						{
							/** Concat TempString and value */
							sprintf(TempChar
									,"%llu"
									,va_arg( vl, TU64_t ));

							++i;
						}
						else
						{
							/** Concat TempString and value */
							sprintf(TempChar
									,"%u"
									,va_arg( vl, TU32_t ));
						}
					break;
					case 'f':
						/** Concat TempString and value */
						sprintf(TempChar
								,"%f"
								,va_arg( vl, double ));
					break;
					case 's':
						{
							/** Concat TempString and value */
							TS08_t Arg = va_arg( vl, TS08_t );

							if(Arg)
								sprintf(TempChar, "%s", Arg);
						}
						break;
					case 'w':
						{
							/** Concat TempString and value */
							TS16_t Arg = va_arg( vl, TS16_t );
							
							if(Arg)
								wcstombs(	TempChar, Arg, VarListSTRMaxSize);
						}
						break;
					default:
						break;
				}
				
				Temp += TempChar;
				StringStartPoint = i + 1;

				MACRO_MEMSET(TempChar, sizeof(TempChar));
			}

			/** Check if a string exist after last %. */
			if(*(pComments + StringStartPoint) != NULL)
				Temp += pComments + StringStartPoint;
		}
	}

	return Temp;//CMyString<char>(Temp);
}
template <>
inline CMyString<wchar_t> CMyString<wchar_t>::GetVarList(va_list vl, const wchar_t * pComments,...)
{
	TU32_t i = 0;
	TU32_t StringStartPoint = 0;
	CMyString<wchar_t> Temp;

	const TU32_t VarListSTRMaxSize = 500;
	TC16_t TempChar [VarListSTRMaxSize] = {0};

	if(pComments)
	{
		if(vl)
		{
			// Step through the list.
			for( i = 0; pComments[i] != L'\0'; ++i ) 
			{
				if(pComments[i] != L'%')
					continue;
				
				if(i > StringStartPoint)
					Temp.AddString((i - StringStartPoint), pComments + StringStartPoint);

				++i;

				/** Check value type %. */
				switch( pComments[i] ) 
				{   
					case 'c':
						/** Concat TempString and value */
						wsprintf(TempChar
								,L"%c"
								,va_arg( vl, TC16_t ));
					break;
					case 'i':
					case 'd':
						/** Concat TempString and value */
						wsprintf(TempChar
								,L"%i"
								,va_arg( vl, TI32_t ));
					break;
					case 'u':
						if(pComments[i + 1] == 'l')
						{
							/** Concat TempString and value */
							wsprintf(TempChar
									,L"%llu"
									,va_arg( vl, TU64_t ));

							++i;
						}
						else
						{
							/** Concat TempString and value */
							wsprintf(TempChar
									,L"%u"
									,va_arg( vl, TU32_t ));
						}
					break;
					case 'f':
						/** Concat TempString and value */
						wsprintf(TempChar
								,L"%f"
								,va_arg( vl, double ));
					break;
					case 's':
						{
							/** Concat TempString and value */
							TS08_t Arg = va_arg( vl, TS08_t );

							if(Arg)
								mbstowcs(TempChar, Arg, VarListSTRMaxSize);
						}
						break;
					case 'w':
						{
							/** Concat TempString and value */
							TS16_t Arg = va_arg( vl, TS16_t );
							
							if(Arg)
								wsprintf(TempChar, Arg, VarListSTRMaxSize);
						}
						break;
					default:
						break;
				}
				
				Temp += TempChar;
				StringStartPoint = i + 1;

				MACRO_MEMSET(TempChar, sizeof(TempChar));
			}

			/** Check if a string exist after last %. */
			if(*(pComments + StringStartPoint) != NULL)
				Temp += pComments + StringStartPoint;
		}
	}

	return Temp;//CMyString<wchar_t>(Temp);
}
#endif