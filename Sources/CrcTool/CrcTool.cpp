#include "CrcTool\CrcTool.h"
#include "CommonTools\CommonTools.h"
#include "DirectoryInfo\DirectoryInfo.h"
#include <boost/crc.hpp>
#include "LinkedList\LinkedList.h"
#include "FileInfo\FileInfo.h"
using namespace boost;

TU16_t CCrcTool::GetCRC16(TS08_t pBuffer, TU32_t BufferSize)
{

	TU16_t Result = 0;
	crc_16_type  CRCCalculator;

#if defined __cplusplus_cli
	for(TU08_t I = 0 ; I < Buffer->Length; I++)
		CRCCalculator.process_byte(Buffer[I]);
#else
	CRCCalculator.process_bytes(pBuffer, BufferSize * sizeof(*pBuffer));
#endif
	Result = CRCCalculator.checksum();

	return Result;
}
#if defined __cplusplus_cli
TU16_t CCrcTool::GetCRC16(FileStream ^ Fs)
#else
TU16_t CCrcTool::GetCRC16(streambuf * pbuf)
#endif
{
	TU16_t Result = 0;
	crc_16_type  CRCCalculator;
#if defined __cplusplus_cli

	while(Fs->Position < Fs->Length)
		CRCCalculator.process_byte(Fs->ReadByte());

#else
	if(pbuf)
	{
#if defined (PRINT_CRC_DETAILS)
		while (pbuf->sgetc()!=EOF)
		{
			unsigned char Byte = pbuf->sbumpc();
			CRCCalculator.process_byte(Byte);
			printf("Byte = %c -> CRC = %s\n",Byte, CCommonTools::ToBinaryString((TU16_t)CRCCalculator.checksum()));
		}
#else
		while (pbuf->sgetc()!=EOF)
			CRCCalculator.process_byte(pbuf->sbumpc());
#endif
		
	}
	#endif

	Result = CRCCalculator.checksum();

	return Result;

}
TU16_t CCrcTool::GetCRC16(TS08_t PathAndFileName)
{
	TU16_t Result = 0;
#if defined __cplusplus_cli
	FileInfo FI(PathAndFileName);

	if(FI.Exists)
	{
		FileStream ^ FS = FI.Open(FileMode::Open);
		
		Result = GetCRC16(FS);

		FS->Close();
	}
#else
	streambuf * pbuf = NULL;

	if(PathAndFileName)
	{
		ifstream istr (PathAndFileName);
		
		Result = GetCRC16(istr.rdbuf());

		istr.close();
	}
#endif
	return Result;
}
TU16_t CCrcTool::GetDirectoryCRC16(TS08_t DirectoryPath)
{
	TU16_t Result = 0;

	if(DirectoryPath)
	{
		CDirectoryInfo Directory(DirectoryPath);
		Result = GetDirectoryCRC16(&Directory);
	}
	return Result;
}
TU16_t CCrcTool::GetDirectoryCRC16(CDirectoryInfo *pDirectory)
{
	TU16_t Result = 0;
	crc_16_type  CRCCalculator;

	if(pDirectory)
	{
		//pDirectory->Refresh();
		
		for(TU32_t i = 0 ; i < pDirectory->GetDirectories().Count() ; ++i)
		{
			TU16_t CRC = GetDirectoryCRC16(pDirectory->GetDirectories()[i]);
			CRCCalculator.process_bytes(&CRC, 2);
		}
		for(TU32_t i = 0 ; i < pDirectory->GetFiles().Count() ; ++i)
		{
			CFileInfo *pFile = pDirectory->GetFiles()[i];
			pFile->RetrieveCRC16();
			TU16_t CRC = pFile->GetCRC();
			CRCCalculator.process_bytes(&CRC, 2);
		}

		Result = CRCCalculator.checksum();
	}
	return Result;
}