#ifndef CSERIALPORT_H
#define CSERIALPORT_H
#include "Common\MyTypes.h"
//#include <windows.h>
#include "SerialPort\SerialPort_Config.h"
//#include "BaseClient.h"
#include "BaseConnectedClient\BaseConnectedClient.h"

/** Details : http://www.guill.net/index.php?cat=6&prg=11 */

/**********************************************************************************************//**
 * \typedef	void (*pDataReceived)(void * pToObj, TU08_t * pBuffer, TU32_t NbBytes)
 *
 * \brief	Defines an alias representing the nb in bytes.
 **************************************************************************************************/

typedef void (*pDataReceived)(void * pToObj, TU08_t * pBuffer, TU32_t NbBytes);
class CLog;
class CThread;
class CSerialPort_BaseConfig;

class CSerialPort : public CBaseConnectedClient //public CBaseClient
{
public:
	CSerialPort(TU16_t SerialPortNumber
				//, CBaseClientDelegate * pDelegate = NULL
				, CBaseConnectedClient_Delegate * pDelegate = NULL
				, CSerialPort_BaseConfig * pCSerialConfiguration = NULL
				, CLog *pLog = NULL
				, SerialPort_Config_DesiredAccess_t DesiredAccess = SerialPort_Config_DesiredAccess_Both_ReadWrite
				, SerialPort_Config_ShareMode_t Sharemode = SerialPort_Config_ShareMode_ExclusiveAccess
				, SerialPort_Config_Action_t SerialAction = SerialPort_Config_Action_OpenExisting);

	~CSerialPort();
	///** Delegate */
	//inline void SetDelegate(CBaseConnectedClient_Delegate/*CBaseClientDelegate*/ * pDelegate){m_pDelegate = pDelegate;};

	/** Configuration */
	inline CSerialPort_BaseConfig * SerialConfiguration(){return m_pSerialConfiguration;};
	bool SetNewConfiguration(CSerialPort_BaseConfig & SerialConfig);

	/** Timeouts */
	bool SetTimeouts(	TU32_t ReadInterCharTimeout = MAXDWORD
						, TU32_t ReadConstantTimeout = 200
						, TU32_t ReadMultiplierTimeout = 1
						, TU32_t WriteConstantTimeout = 200
						, TU32_t WriteMultiplierTimeout = 1);
	
	/** Port Number */
	inline TU16_t PortNumber(){return m_PortNumber;}
	inline TS16_t ComString(){return (TS16_t)&m_Com_STR;}
	/** Desired Access */
	inline SerialPort_Config_DesiredAccess_t DesiredAccess(){return m_DesiredAccess;};
	/** Shared Mode */
	inline SerialPort_Config_ShareMode_t ShareMode(){return m_Sharemode;};
	/** Serial Mode */
	inline SerialPort_Config_Action_t SerialAction(){return m_SerialAction;};

#if defined (_DEBUG)
	inline HANDLE SerialPortHandle(){return m_hSerialPort;}
#endif
private:
	void SetNewPort(TU16_t PortNumber);
	/** Port Number */
	TU16_t m_PortNumber;
	TC16_t m_Com_STR [10];
	/** Timeouts */
	bool SetTimeouts(COMMTIMEOUTS & CommTimeoutStructure);
	COMMTIMEOUTS GetTimeouts();

	/** Configuration */
	bool SetNewConfiguration(DCB & DCbStructure);
	DCB GetCurrentConfig();
	/** Desired Access */
	SerialPort_Config_DesiredAccess_t m_DesiredAccess;
	/** Shared Mode */
	SerialPort_Config_ShareMode_t m_Sharemode;
	/** Serial Mode */
	SerialPort_Config_Action_t m_SerialAction;

	void Initialise();
	HANDLE m_hSerialPort;   ///< Handle of the serial port

	DCB m_DcbStructure;
	
	COMMTIMEOUTS m_Timeouts;	///< The timeouts

	///** Base Call Backs */
	//void Event_ConnectionStateChanged(BaseConnectedClientStatus_t NewConnectionState);
	//void Event_FrameReceived(TU08_t * pBuffer, TU32_t Size, TU32_t SizeWaited);
	//void Event_FrameSendResult(TU08_t * pBuffer, TU32_t BufferSize, TU32_t NBBytesSent, TU32_t FrameIDentifier);

	/** Inherited Virtual Functions */
	bool VWillConnect();
	bool VConnect();
	bool VDidConnect();

	/** DisConnection */
	bool VWillDisConnect();
	bool VDisConnect();
	bool VDidDisConnect();

	TU32_t VSend(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier = NULL); 
	TU32_t VReceive(TU08_t * pOutBuffer, TU32_t outSize); 
	
	/** Delegate */
	//CBaseClientDelegate * m_pDelegate;

	/** Serial Configuration */
	CSerialPort_BaseConfig * m_pSerialConfiguration;
};
#endif