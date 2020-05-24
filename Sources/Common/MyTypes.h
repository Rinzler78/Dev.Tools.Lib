#ifndef INGENICOTYPES_H
#define INGENICOTYPES_H

#pragma region <Types>

#ifndef NULL
#define NULL 0
#endif

#if ! (defined (WIN32) || defined (WINCE))
#define HANDLE void *
#endif

#if ! (defined (WIN32) || defined (WINCE))
typedef struct sockaddr_in SOCKADDR_IN;
#endif

#if ! (defined (WIN32) || defined (WINCE))
typedef int SOCKET;
#endif

#if ! (defined (WIN32) || defined (WINCE))
#define INVALID_SOCKET (SOCKET)0
#endif

#if ! (defined (WIN32) || defined (WINCE))
#define closesocket( socket ) close( socket )
#endif

#if ! (defined (WIN32) || defined (WINCE))
#define DWORD TU32_t
#endif

typedef char TC08_t, TI08_t;
typedef unsigned char TU08_t;
typedef TC08_t * TS08_t;
typedef short int TI16_t;
typedef unsigned short int TU16_t;
typedef wchar_t  TC16_t;
typedef wchar_t *TS16_t;
typedef unsigned int TU32_t;
typedef int TI32_t;
typedef double TD32_t;
typedef long long TI64_t;
typedef unsigned long long TU64_t;

#pragma endregion

typedef enum ING_STATUS
{
	/** Generique results */
	ING_STATUS_FAILURE = 0,
	ING_STATUS_SUCCESS,
	/** OS Update Result */
	ING_STATUS_OS_UPDATE_FAILURE,
	ING_STATUS_OS_UPDATE_DO_UPDATE_MANUALLY,
	ING_STATUS_OS_UPDATE_FILE_NOT_EXIST,
	/** EBoot Update */
	ING_STATUS_EBOOT_UPDATE_FAILURE,
	ING_STATUS_EBOOT_UPDATE_DO_UPDATE_MANUALLY,
	ING_STATUS_EBOOT_UPDATE_FILE_NOT_EXIST,
	/** Logo Update */
	ING_STATUS_LOGO_UPDATE_FAILURE,
	ING_STATUS_LOGO_UPDATE_FILE_NOT_EXIST,
	/** Firmware */
	ING_STATUS_FIRMWARE_ALLREADY_UP_TO_DATE,
	ING_STATUS_FIRMWARE_UNAUTHORISED_DOWNGRADE,

	MAX_ING_STATUS
}ING_STATUS_t;  ///< .

const TC08_t ING_STATUS_STR_TAB[MAX_ING_STATUS][50] = ///< .
{
	"Failure"
	, "Success"
	, "Os Update -> Failure"
	, "Os Update -> Do Update Manually"
	, "Os Update failure -> Os file not exist"
	, "EBoot Update -> Failure"
	, "EBoot Update -> Do Update Manually"
	, "EBoot Update failure -> EBoot file not exist"
	, "Logo Update -> Failure"
	, "Logo Update failure -> Logo file not exist"
	, "Firmware Allready Up To Date"
	, "UnAuthorised Download"
};

#define TEMPLAT_DECL template <class T>
#define TEMPLAT_TYPE_DECL template <typename T>

typedef enum StringType
{
	CHAR_TYPE = 0,
	WCHAR_TYPE,
	MAX_StringType
}StringType_t;  ///< .
#include "stdio.h"

#pragma region <Ingenico MACCRO>
#define MACRO_DELET_TAB(x) if(x){delete [] x; x = NULL;}
#define MACRO_DELET(x) if(x){delete x; x = NULL;}


#define MACRO_MEMSET(Element,Size)if(Element){memset(Element,0 , Size);}
#pragma endregion

#include <typeinfo>
static const std::type_info & CharType = typeid(char);
static const std::type_info & WCharType = typeid(wchar_t);

#endif