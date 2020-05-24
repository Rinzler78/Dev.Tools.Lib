#include "GPS.h"
#include "SerialPort\SerialPort.h"
#include "CommonTools\CommonTools.h"
#include "GPS\NMEA_Frame.h"
#include "Log\Log.h"
#include "SerialPort\SerialPort_BaseConfig.h"
#define NMEA_FRAME_BUFFER_SIZE 256

CGPS::CGPS(TU16_t GPSSerialPort, SerialPort_Config_Baudrate_t Baudrate, CLog * pLog)
:m_pGPSSerialPort(NULL)
, m_pLog(pLog)
, m_FixeQualification(NMEA_GGA_FixeQualification_Invalid)
, m_NumberOfSat(0)
, m_DOP(0)
, m_Altitude(0)
, m_Height(0)
, m_InAcquisition(false)
, m_UpdatePVTCallBack(NULL)
{
	MACRO_MEMSET(&m_PVT, sizeof(m_PVT));
	/** Generate Configuration */
	CSerialPort_BaseConfig GPSConfig(Baudrate);
	m_pGPSSerialPort = new CSerialPort(GPSSerialPort
										, this
										, &GPSConfig
										, m_pLog);
}
bool CGPS::StartAcquisition(pUpdatePVTCallBack UpdatePVTCallBack)
{
	bool Result = false;
	if(!m_InAcquisition)
	{
		if(!m_pGPSSerialPort->Connected())
			m_pGPSSerialPort->ConnectAsync();

		m_UpdatePVTCallBack = UpdatePVTCallBack;

		m_InAcquisition = true;

		if(m_pLog)
			m_pLog->Log(NOCOMMENTS, "Start Acquisition on %w",m_pGPSSerialPort->ComString());

	}
	return Result;
}
bool CGPS::StopAcquisition()
{
	bool Result = false;
	if(m_InAcquisition)
	{
		if(m_pLog)
			m_pLog->Log(NOCOMMENTS,"Stop Acquisition");
		if(m_pGPSSerialPort)
		{
			Result = m_pGPSSerialPort->DisConnectAsync();
			m_InAcquisition = !Result;
		}
	}

	return Result;
}
CGPS::~CGPS()
{
	StopAcquisition();
	MACRO_DELET(m_pGPSSerialPort)

}
bool CGPS::GetInfos(CNMEA_FRAME * pNMEAFrame)
{
	bool Result = false;
	
	if(pNMEAFrame)
	{
		switch(pNMEAFrame->FrameID())
		{
			case GPS_FrameID_GGA:
				m_PVT.Time.tm_hour = ((CNMEA_GGA *)pNMEAFrame)->FixeTime().tm_hour;
				m_PVT.Time.tm_min = ((CNMEA_GGA *)pNMEAFrame)->FixeTime().tm_min;
				m_PVT.Time.tm_sec = ((CNMEA_GGA *)pNMEAFrame)->FixeTime().tm_sec;

				m_PVT.Position = ((CNMEA_GGA *)pNMEAFrame)->Position();

				m_NumberOfSat = ((CNMEA_GGA *)pNMEAFrame)->NumberOfSate();
				m_DOP = ((CNMEA_GGA *)pNMEAFrame)->DOP();
				m_FixeQualification = ((CNMEA_GGA *)pNMEAFrame)->FixeQualification();
				m_Height = ((CNMEA_GGA *)pNMEAFrame)->Height();
				m_Altitude = ((CNMEA_GGA *)pNMEAFrame)->Altitude();
				break;
			case GPS_FrameID_GLL:
				m_PVT.Position = ((CNMEA_GLL *)pNMEAFrame)->Position();

				m_PVT.Time.tm_hour = ((CNMEA_GLL *)pNMEAFrame)->FixeTime().tm_hour;
				m_PVT.Time.tm_min = ((CNMEA_GLL *)pNMEAFrame)->FixeTime().tm_min;
				m_PVT.Time.tm_sec = ((CNMEA_GLL *)pNMEAFrame)->FixeTime().tm_sec;
				break;
			case GPS_FrameID_GSA:
				break;
			case GPS_FrameID_GSV:
				m_NumberOfSat = ((CNMEA_GSV *)pNMEAFrame)->NumberOfSat();
				break;
			case GPS_FrameID_VTG:
				m_PVT.Speed = ((CNMEA_VTG *)pNMEAFrame)->SpeedFromGround_InKmH();
				m_PVT.Heading = ((CNMEA_VTG *)pNMEAFrame)->HeadTInDegrees();
				break;
			case GPS_FrameID_RMC:
				m_PVT.Time = ((CNMEA_RMC *)pNMEAFrame)->FixeTime();
				m_PVT.Position = ((CNMEA_RMC *)pNMEAFrame)->Position();
				break;
			default:
				break;
		}
	}

	return Result;
}
bool CGPS::GetInfos(TU08_t * pBuffer, TU32_t NbBytes)
{
	bool Result = false;
	TI32_t Index = 0;
	TS08_t Frame = NULL;
	static TC08_t OldPartOfFrame [100] = {0};
	TU32_t OldPartOfframeLength = 0;
	TU32_t StartStringPos = 0;
	TU32_t FrameLength = 0;
	TI32_t FrameStartPos = -1;
	TI32_t FrameEndPos = -1;
	TU32_t PartFrameLength = 0;

	if(pBuffer && NbBytes > 0)
	{
		Result = true;
		//Frames Exemples
		//0x0054e178 "207.032,V,,,,,,,2910¤¤¤¤лллллллл"
		//

		/** Search for NMEA_BEGIN_LINE in Buffer Received */
		Index = CCommonTools::FindFirstIndex((TS08_t)pBuffer, NMEA_BEGIN_LINE, NbBytes);
		if(Index > -1)
		{
			/** A frame begin at FrameStartPos */
			FrameStartPos = Index;
			/** Search for the end of next frame */
			PartFrameLength = NbBytes - FrameStartPos;

			if(FrameStartPos > 0)
			{
				/** Treat older Frame */
				if(OldPartOfFrame[0])
				{
					OldPartOfframeLength = strlen(OldPartOfFrame);

					/** Copy the end of frame to the old frame buffer */
					memcpy(&OldPartOfFrame[OldPartOfframeLength], pBuffer, FrameStartPos);
					GetInfos((TU08_t *)OldPartOfFrame, OldPartOfframeLength + FrameStartPos);
					MACRO_MEMSET(OldPartOfFrame, sizeof(OldPartOfFrame));
				}

				/** Treat New frame */
				GetInfos(pBuffer + FrameStartPos, NbBytes - FrameStartPos);
			}
			else
			{
				/** search for the end of frame */
				Index = CCommonTools::FindFirstIndex((TS08_t)pBuffer + FrameStartPos, NMEA_EOF, PartFrameLength);
				if(Index > -1)
				{
					Frame = new TC08_t[Index + 1];
					MACRO_MEMSET(Frame, Index + 1);
					memcpy(Frame, pBuffer + FrameStartPos, Index);

					/** Complete frame found */
					FrameFound(this,Frame,0);
					
					MACRO_DELET_TAB(Frame)
					
					/** Treat rest of frame */
					GetInfos( pBuffer + FrameStartPos + Index, NbBytes - FrameStartPos - Index);

					if(m_UpdatePVTCallBack)
						m_UpdatePVTCallBack(PVT());
				}
				else
				{
					/** Save to old part frame Buffer */
					memcpy(OldPartOfFrame, pBuffer, PartFrameLength);
				}
			}
		}
		else
		{
			/** It's probably another part of the old frame */
			if(OldPartOfFrame[0])
			{
				OldPartOfframeLength = strlen(OldPartOfFrame);//Current Old Frame Size
				memcpy(&OldPartOfFrame[OldPartOfframeLength], pBuffer, NbBytes);
			}
		}
	}

	return Result;
}
void CGPS::Event_FrameReceived(TU08_t * pBuffer, TU32_t Size, TU32_t SizeWaited)
{
	if(pBuffer && Size > 0)
	{
		GetInfos(pBuffer, Size);
		/** Retake Another frame */
		m_pGPSSerialPort->ReceiveAsync(NMEA_FRAME_BUFFER_SIZE);
	}
}
void CGPS::Event_ConnectionStateChanged(BaseConnectedClientStatus_t NewConnectionState)
{
	switch(NewConnectionState)
	{
		case BaseConnectedClientStatus_Connected:
			/** Begin Data Reception */
			m_pGPSSerialPort->ReceiveAsync(NMEA_FRAME_BUFFER_SIZE);
			break;
		case BaseConnectedClientStatus_ConnectionFailure:
		case BaseConnectedClientStatus_Disconnected:
			StopAcquisition();
			break;
	}

}
void CGPS::Event_FrameSendResult(TU08_t * pBuffer, TU32_t BufferSize, TU32_t NBBytesSent, TU32_t FrameIDentifier)
{
}