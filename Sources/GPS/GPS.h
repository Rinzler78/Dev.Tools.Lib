#ifndef CGPS_H
#define CGPS_H
#include "Common\MyTypes.h"
#include "GPS_Common.h"
#include "GPS\NMEA_GGA.h"
#include "GPS\NMEA_GLL.h"
#include "GPS\NMEA_GSA.h"
#include "GPS\NMEA_GSV.h"
#include "GPS\NMEA_VTG.h"
#include "GPS\NMEA_RMC.h"
//#include "BaseClient_Defs.h"
//#include "BaseClientDelegate.h"
#include "BaseConnectedClient\BaseConnectedClient_Delegate.h"

#include "SerialPort\SerialPort_Config.h"

typedef void (*pUpdatePVTCallBack)(GPS_PVT_t NewPVT);
/** ref http://www.gpspassion.com/forumsen/topic.asp?TOPIC_ID=17661 */
class CSerialPort;
class CLog;
class CNMEA_FRAME;


class CGPS : private CBaseConnectedClient_Delegate
{
public:
	CGPS(TU16_t GPSSerialPort, SerialPort_Config_Baudrate_t Baudrate = SerialPort_Config_Baudrate_9600, CLog * pLog = NULL);
	~CGPS();
	bool StartAcquisition(pUpdatePVTCallBack UpdatePVTCallBack = NULL);
	bool StopAcquisition();
	bool GetInfos(CNMEA_FRAME * pNMEAFrame);
	inline CLog * Log(){return m_pLog;}
	inline GPS_PVT_t PVT(){return m_PVT;}
	inline bool IsInAcquisition(){return m_InAcquisition;}
private:
	bool GetInfos(TU08_t * pBuffer, TU32_t Size);
	pUpdatePVTCallBack m_UpdatePVTCallBack; ///< The update pvt call back
	bool m_InAcquisition;   ///< true to in acquisition
	CSerialPort * m_pGPSSerialPort; ///< The gps serial port
	CLog * m_pLog;  ///< The log
	GPS_PVT_t m_PVT;	///< The pvt
	NMEA_GGA_FixeQualification_t m_FixeQualification;   ///< The fixe qualification
	TU08_t m_NumberOfSat; ///< Number of sats
	TD32_t m_DOP; ///< The dop
	TD32_t m_Altitude;	///< The altitude
	TD32_t m_Height;  ///< The height

	/** Base Call Backs */
	void Event_ConnectionStateChanged(BaseConnectedClientStatus_t NewConnectionState);
	void Event_FrameSendResult(TU08_t * pBuffer, TU32_t BufferSize, TU32_t NBBytesSent, TU32_t FrameIDentifier);
	void Event_FrameReceived(TU08_t * pBuffer, TU32_t Size, TU32_t SizeWaited);
};
#endif