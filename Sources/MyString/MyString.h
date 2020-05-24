#ifndef CMYSTRING_H
#define CMYSTRING_H

#include "Common\MyTypes.h"
#include "MyString\MyString_Defs.h"

TEMPLAT_DECL
class CLinkedList;

TEMPLAT_TYPE_DECL
class CMyString
{
public:
	~CMyString(void);

	CMyString();
	CMyString(T * pString);
	CMyString(const T * pString);
	CMyString(TU32_t Size, T * pString);
	CMyString(TU32_t Size, const T * pString);
	CMyString(const CMyString<T> &);
	CMyString(CMyString<T> *);

	template<typename ST> CMyString(ST * pString);
	template<typename ST> CMyString(const ST * pString);
	template<typename ST> CMyString(TU32_t Size, ST * pString);
	template<typename ST> CMyString(TU32_t Size, const ST * pString);
	template<typename ST> CMyString(const CMyString<ST> &);
	template<typename ST> CMyString(CMyString<ST> *);

	/** operators */
	/** Equality */
	bool operator == (const CMyString<T> & SrcString)const;
	bool operator == (const T * pString)const;
	bool operator == (T * pString)const;
	template<typename ST> bool operator == (const CMyString<ST> & SrcString)const;
	template<typename ST> bool operator == (const ST * pString)const;
	template<typename ST> bool operator == (ST * pString)const;

	bool operator != (const CMyString<T> & SrcString)const;
	bool operator != (const T * pString)const;
	bool operator != (T * pString)const;
	template<typename ST> bool operator != (const CMyString<ST> & SrcString)const;
	template<typename ST> bool operator != (const ST * pString)const;
	template<typename ST> bool operator != (ST * pString)const;
	
	/** Add String */
	void operator += (const CMyString<T> & SrcString);
	void operator += (const T * pString);
	void operator += (T * pString);

	template<typename ST> void operator += (const CMyString<ST> & SrcString);
	template<typename ST> void operator += (const ST * pString);
	template<typename ST> void operator += (ST * pString);
	
	void AddString(TU32_t Len, T * pString);
	void AddString(TU32_t Len, const T * pString);
	void AddString(const T *  pComments,...);

	template<typename ST> void AddString(TU32_t Size, ST * pString);
	template<typename ST> void AddString(TU32_t Size, const ST * pString);

	CMyString<T> operator + (const T * pString);
	CMyString<T> operator + (const CMyString<T> & SrcString);
	template<typename ST> CMyString<T> operator + (const ST * pString);
	template<typename ST> CMyString<T> operator + (const CMyString<ST> & SrcString);

	/** Set String */
	void operator = (const T * pString);
	void operator = (T * pString);
	void operator = (const CMyString<T> & SrcString);

	template<typename ST> void operator = (const ST * pString);
	template<typename ST> void operator = (ST * pString);
	template<typename ST> void operator = (const CMyString<ST> & SrcString);
	
	void SetString(TU32_t Size, T * pString);
	void SetString(TU32_t Size, const T * pString);
	void SetString(const T * pComments,...);

	template<typename ST> void SetString(TU32_t Size, ST * pString);
	template<typename ST> void SetString(TU32_t Size, const ST * pString);

	/** Getters */
	inline TU32_t Length() const{return m_Length;}
	T * String()const{return m_pString;}
	T operator [] (TU32_t Index)const {return (Index <= m_Length ? m_pString[Index] : 0);};

	/** Split */
	CLinkedList<CMyString<T> *> * Split(T Separator)const;
	CLinkedList<CMyString<T> *> * Split(T * pSeparatorString, TU32_t SeparatorStringSize)const;
	void Split(T Separator, CMyStringDelegate<T> & Delegate)const;
	void Split(T * pSeparatorString, TU32_t SeparatorStringSize, CMyStringDelegate<T> & Delegate)const;

	/** Find */
	TI32_t FindFirst(const T CharSearch)const;
	inline static TI32_t FindFirst(const T * pString, const TU32_t StringLen, const T CharSearch);
	
	TI32_t FindLast(const T CharSearch)const;
	inline static TI32_t FindLast(const T * pString, const TU32_t StringLen, const T CharSearch);

	TI32_t FindFirst(const T * pStringSearch, TU32_t StringSearchSize)const;
	TI32_t FindLast(const T * pStringSearch, TU32_t StringSearchSize)const;

	/** Modification */
	bool SupressBefore(T Char);
	bool SupressBefore(T * Char);
	bool SupressBefore(const T * Char);

	bool SupressAfter(T Char);
	bool SupressAfter(T * Char);
	bool SupressAfter(const T * Char);

	bool Replace(T CurrentChar, T NewChar);

	/** String Tests */
	inline bool IsEndBy(T Char) const {return (m_Length > 0 ? m_pString[m_Length - 1] == Char:false);}
	inline bool IsBeginBy(T Char) const {return (m_Length > 0 ? m_pString[0] == Char:false);}
	
	inline static CMyString<T> GetString(const T * pPattern, ...);
	inline static CMyString<T> GetVarList(va_list vl, const T *  pComments,...);
private:
	T * m_pString;
	TU32_t m_Length;
	TU32_t m_BufferLength;

	/** String Length */
	 template<typename ST> inline static TU32_t StringLength(ST * pString);
	
	/** Set String */
	template<typename ST>
	void SetString(ST * pString);
	
	void SetString(T * pString);
	void SetString(const T * pString);
	
	/** Add String */
	template<typename ST>
	void AddString(ST * pString);
	template<typename ST>
	void AddString(const ST * pString);
	
	void AddString(T * pString);
	void AddString(const T * pString);

	/** Equality */
	bool IsEqual(const T * pString)const;
	bool IsEqual(TU32_t Len, const T * pString)const;
	bool IsEqual(const CMyString<T> & String)const;

	template<typename ST>
	bool IsEqual(const ST * pString)const;
	template<typename ST>
	bool IsEqual(TU32_t Len, const ST * pString)const;
	template<typename ST>
	bool IsEqual(const CMyString<ST> & String)const;
};
#include "MyString\MyString.hpp"
#include "MyString\MyString_AddString.hpp"
#include "MyString\MyString_SetString.hpp"
#include "MyString\MyString_StringLength.hpp"
#include "MyString\MyString_StringTests.hpp"
#endif