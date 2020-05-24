#ifndef CCRCTOOL_H
#define CCRCTOOL_H
#include "Common\MyTypes.h"
#if defined __cplusplus_cli
using namespace System::IO;
#else
#include <iostream>
#include <fstream>
using namespace std;
#endif

#if defined (_DEBUG)
//#define PRINT_CRC_DETAILS
#endif

/**********************************************************************************************//**
 * \class	CCrcTool
 *
 * \brief	Crc tool.
 *
 * \author	Bleclere
 * \date	12/03/2012
 **************************************************************************************************/

class CDirectoryInfo;

/**********************************************************************************************//**
 * \class	CCrcTool
 *
 * \brief	Crc tool.
 *
 * \author	Bleclere
 * \date	12/03/2012
 **************************************************************************************************/

class CCrcTool
{
public:
	#if defined __cplusplus_cli

	/**********************************************************************************************//**
	 * \fn	static TU16_t CCrcTool::GetCRC16(array<TU08_t> ^ Buffer);
	 *
	 * \brief	Gets a crc 16.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param [in,out]	Buffer	If non-null, the buffer.
	 *
	 * \return	The crc 16.
	 **************************************************************************************************/

	static TU16_t GetCRC16(array<TU08_t> ^ Buffer);
#else

	/**********************************************************************************************//**
	 * \fn	static TU16_t CCrcTool::GetCRC16(TS08_t pBuffer, TU32_t BufferSize);
	 *
	 * \brief	Gets a crc 16.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	pBuffer   	The buffer.
	 * \param	BufferSize	Size of the buffer.
	 *
	 * \return	The crc 16.
	 **************************************************************************************************/

	static TU16_t GetCRC16(TS08_t pBuffer, TU32_t BufferSize);
#endif
#if defined __cplusplus_cli

	/**********************************************************************************************//**
	 * \fn	static TU16_t CCrcTool::GetCRC16(FileStream ^ Fs);
	 *
	 * \brief	Gets a crc 16.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param [in,out]	Fs	If non-null, the file system.
	 *
	 * \return	The crc 16.
	 **************************************************************************************************/

	static TU16_t GetCRC16(FileStream ^ Fs);
#else

	/**********************************************************************************************//**
	 * \fn	static TU16_t CCrcTool::GetCRC16(streambuf * pbuf);
	 *
	 * \brief	Gets a crc 16.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param [in,out]	pbuf	If non-null, the pbuf.
	 *
	 * \return	The crc 16.
	 **************************************************************************************************/

	static TU16_t GetCRC16(streambuf * pbuf);
#endif

	/**********************************************************************************************//**
	 * \fn	static TU16_t CCrcTool::GetCRC16(TS08_t PathAndFileName);
	 *
	 * \brief	Gets a crc 16.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	PathAndFileName	Filename of the path and file.
	 *
	 * \return	The crc 16.
	 **************************************************************************************************/

	static TU16_t GetCRC16(TS08_t PathAndFileName);

	/**********************************************************************************************//**
	 * \fn	static TU16_t CCrcTool::GetDirectoryCRC16(TS08_t DirectoryPath);
	 *
	 * \brief	Gets a directory crc 16.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param	DirectoryPath	Full pathname of the directory file.
	 *
	 * \return	The directory crc 16.
	 **************************************************************************************************/

	static TU16_t GetDirectoryCRC16(TS08_t DirectoryPath);

	/**********************************************************************************************//**
	 * \fn	static TU16_t CCrcTool::GetDirectoryCRC16(CDirectoryInfo *pDirectory);
	 *
	 * \brief	Gets a directory crc 16.
	 *
	 * \author	Bleclere
	 * \date	23/02/2012
	 *
	 * \param [in,out]	pDirectory	If non-null, pathname of the directory.
	 *
	 * \return	The directory crc 16.
	 **************************************************************************************************/

	static TU16_t GetDirectoryCRC16(CDirectoryInfo *pDirectory);
private:
};
#endif