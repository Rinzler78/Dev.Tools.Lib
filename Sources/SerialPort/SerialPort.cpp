#include "SerialPort\SerialPort.h"
#include "CommonTools\CommonTools.h"
#include "SerialPort_Config_Private.h"
#include "Log\Log.h"
#include "FileInfo\FileInfo.h"
#include "Thread\Thread.h"
#include "Mutex\Mutex.h"
#include "Clock\Clock.h"
#include "SerialPort\SerialPort_BaseConfig.h"
#include "SerialPort_Config_FlowControl_XOnXOff.h"
#include "SerialPort_Config_FlowControl_DTR_DSR.h"
#include "SerialPort_Config_FlowControl_RTS_CTS.h"

CSerialPort::CSerialPort(TU16_t SerialPortNumber
						//, CBaseClientDelegate * pDelegate
						, CBaseConnectedClient_Delegate * pDelegate
						, CSerialPort_BaseConfig * pCSerialConfiguration
						, CLog *pLog
						, SerialPort_Config_DesiredAccess_t DesiredAccess
						, SerialPort_Config_ShareMode_t Sharemode
						, SerialPort_Config_Action_t SerialAction)
: CBaseConnectedClient(BaseClientConnectionType_Serial, pDelegate, pLog)//CBaseClient(ConnectionType_Serial, this, pLog)
, m_PortNumber(SerialPortNumber)
, m_hSerialPort(INVALID_HANDLE_VALUE)
//, m_pDelegate(pDelegate)
, m_pSerialConfiguration(NULL)
, m_DesiredAccess(DesiredAccess)
, m_Sharemode(Sharemode)
, m_SerialAction(SerialAction)
{
	Initialise();
	SetNewPort(SerialPortNumber);
	if(pCSerialConfiguration)
		SetNewConfiguration(*pCSerialConfiguration);
}
CSerialPort::~CSerialPort()
{
	DisConnect();//AddAsyncAction(BaseClient_Event_Exit, 0, true);
	MACRO_DELET(m_pSerialConfiguration)
}
void CSerialPort::SetNewPort(TU16_t PortNumber)
{
	/** Port Number STR */
	MACRO_MEMSET(&m_Com_STR, sizeof(m_Com_STR));
	m_PortNumber = PortNumber;

	wsprintfW(	m_Com_STR
#if ! defined (UNDER_CE)
				, L"\\\\.\\COM%u"
#else
				, L"COM%u"
#endif
				, m_PortNumber);
}
bool CSerialPort::SetNewConfiguration(CSerialPort_BaseConfig & SerialConfig)
{
	bool Result = false;

	if(!m_pSerialConfiguration)
		m_pSerialConfiguration = new CSerialPort_BaseConfig(SerialConfig);
	else
		m_pSerialConfiguration->SetNewConfiguration(SerialConfig);

	MACRO_MEMSET(&m_DcbStructure, sizeof(m_DcbStructure));
	m_DcbStructure.DCBlength = sizeof(m_DcbStructure);

	GetCommState(m_hSerialPort, &m_DcbStructure);

	m_DcbStructure.BaudRate = SerialPort_Config_Baudrate_Values_Tab[SerialConfig.BaudRate()];
	m_DcbStructure.Parity = SerialPort_Config_Parity_Values_Tab[SerialConfig.Parity()];
	m_DcbStructure.StopBits = SerialPort_Config_StopBits_Values_Tab[SerialConfig.StopBits()];
	m_DcbStructure.ByteSize = SerialPort_Config_DataBits_Values_Tab[SerialConfig.DataBits()];
	m_DcbStructure.fParity = false;//!(m_DcbStructure.Parity == SerialPort_Config_Parity_None);
	m_DcbStructure.fBinary = true;/** Must be true for Windows */

	if(SerialConfig.FlowControl())
	{
		switch(SerialConfig.FlowControl()->FlowControl())
		{
			case SerialPort_Config_FlowControl_None:
				break;
			case SerialPort_Config_FlowControl_XOn_XOff:
				{
					CSerialPort_Config_FlowControl_XOnXOff * pFlowControl = (CSerialPort_Config_FlowControl_XOnXOff *) SerialConfig.FlowControl();
					m_DcbStructure.XonChar = pFlowControl->XOnChar();
					m_DcbStructure.XoffChar = pFlowControl->XOffChar();
					m_DcbStructure.XonLim = pFlowControl->XOnLim();
					m_DcbStructure.XoffLim = pFlowControl->XOffLim();
					m_DcbStructure.fInX = true;
					m_DcbStructure.fOutX = true;
					m_DcbStructure.fTXContinueOnXoff = pFlowControl->ContinueAfterXOff();
				}
				break;
			case SerialPort_Config_FlowControl_DTR_DSR:
				{
					CSerialPort_Config_FlowControl_DTR_DSR * pFlowControl = (CSerialPort_Config_FlowControl_DTR_DSR *) SerialConfig.FlowControl();
					m_DcbStructure.fDsrSensitivity = pFlowControl->DSRSensitivity();
					m_DcbStructure.fOutxDsrFlow = pFlowControl->OutDSRFlow();
					m_DcbStructure.fDtrControl =  SerialPort_Config_DTR_DSR_Control_Values_Tab[pFlowControl->DTRControl()];
				}
				break;
			case SerialPort_Config_FlowControl_RTS_CTS:
				{
					CSerialPort_Config_FlowControl_RTS_CTS * pFlowControl = (CSerialPort_Config_FlowControl_RTS_CTS *) SerialConfig.FlowControl();
					m_DcbStructure.fOutxCtsFlow = pFlowControl->CTSEnable();
					m_DcbStructure.fRtsControl = SerialPort_Config_RTS_CTS_Control_Values_Tab[pFlowControl->RTSControl()];
				}
				break;
		}
	}
	
	/** Data Events */
	m_DcbStructure.EvtChar = SerialConfig.EventStartChar();
	m_DcbStructure.EofChar = SerialConfig.EventStopChar();

	/** On Parity Error */
	m_DcbStructure.ErrorChar = SerialConfig.ErrorChar();
	m_DcbStructure.fNull = SerialConfig.DiscardNULLChar();
	m_DcbStructure.fErrorChar = SerialConfig.ReplaceParityErrorByErrorChar();
	
	/** On transmission error */
	m_DcbStructure.fAbortOnError = false;
	
	Result = SetNewConfiguration(m_DcbStructure);

	return Result;
}
bool CSerialPort::SetNewConfiguration(DCB & DCbStructure)
{
	bool Result = false;
	m_DcbStructure = DCbStructure;

	if(Connected())
	{
		/** Set Current Port Configuration */
		Result = (SetCommState(m_hSerialPort, &DCbStructure) == TRUE);
		if(!Result)
			if(LogObject())
				LogObject()->Log(ALERT, "Cannot Set Comm State (error code : %u)", GetLastError());
	}

	return Result;
}
bool CSerialPort::SetTimeouts(	TU32_t ReadInterCharTimeout
					, TU32_t ReadConstantTimeout
					, TU32_t ReadMultiplierTimeout
					, TU32_t WriteConstantTimeout
					, TU32_t WriteMultiplierTimeout)
{
	bool Result = false;
	
	m_Timeouts.ReadIntervalTimeout = ReadInterCharTimeout;
	m_Timeouts.ReadTotalTimeoutConstant = ReadConstantTimeout;
	m_Timeouts.ReadTotalTimeoutMultiplier = ReadMultiplierTimeout;
	m_Timeouts.WriteTotalTimeoutConstant = WriteConstantTimeout;
	m_Timeouts.WriteTotalTimeoutMultiplier = WriteMultiplierTimeout;

	Result = SetTimeouts(m_Timeouts);

	return Result;
}
bool CSerialPort::SetTimeouts(COMMTIMEOUTS & CommTimeoutStructure)
{
	bool Result = false;
	
	if(Connected())
	{
		/** Set Current Port Timeout */
		Result = (SetCommTimeouts(m_hSerialPort,&CommTimeoutStructure) == TRUE);
		if(!Result)
			if(LogObject())
				LogObject()->Log(ALERT, "Cannot Set Comm Timeouts");
	}

	return Result;
}
DCB CSerialPort::GetCurrentConfig()
{
	DCB DcbStructure = {0};

	/** Get Current Port Configuration */
	if(!GetCommState(m_hSerialPort,&DcbStructure))
		if(LogObject())
			LogObject()->Log(ALERT, "Cannot Get Comm State");

	return DcbStructure;
}
void CSerialPort::Initialise()
{
	MACRO_MEMSET(&m_DcbStructure, sizeof(m_DcbStructure))
	MACRO_MEMSET(&m_Timeouts,sizeof(m_Timeouts))
	MACRO_MEMSET(&m_Com_STR, sizeof(m_Com_STR))
}
bool CSerialPort::VWillConnect()
{
	return true;
}
bool CSerialPort::VConnect()
{
	bool Result = false;
	
	if(LogObject())
		LogObject()->Log(NOCOMMENTS, "Try to open %w", ComString());

	/** open port in both read Write */
	m_hSerialPort = CreateFileW( ComString()
								, SerialPort_Config_DesiredAccess_Values_Tab[DesiredAccess()]
								, SerialPort_Config_ShareMode_Values_Tab [ShareMode()]
								, 0 /** handle No heritable */
								, SerialPort_Config_Action_Values_Tab[SerialAction()]
								, 0// we want overlapped operation
								, 0// no templates file for COM port...
								);

	Result = (m_hSerialPort != INVALID_HANDLE_VALUE);

	if(Result)
	{
		if(LogObject())
			LogObject()->Log(NOCOMMENTS, "Open %w success", ComString());

		if(!PurgeComm(m_hSerialPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ))
			if(LogObject())
				LogObject()->Log(ALERT, "PurgeComm failure");

		//if(!SetCommMask(m_hSerialPort, 0x00 ))// Disable All Events
		//	if(LogObject())
		//		LogObject()->Log(ALERT, "Set Comm Mask failure");

		if(!SetupComm(m_hSerialPort, 4096, 4096)) // setup device buffers
			if(LogObject())
				LogObject()->Log(ALERT, "Setup Comm failure");
	}
	else
	{
		if(LogObject())
			LogObject()->Log(ALERT, "Open %w Failure (error code : %u)", ComString(), GetLastError());
	}
	
	if(!Result)
	{
		CloseHandle(m_hSerialPort);
		m_hSerialPort = INVALID_HANDLE_VALUE;
	}
	return Result;
}
bool CSerialPort::VDidConnect()
{
	bool Result = false;
#if defined (_DEBUG)
	DCB Temp = GetCurrentConfig();
#endif
	if(m_pSerialConfiguration)
		Result = SetNewConfiguration(m_DcbStructure);
	Result = SetTimeouts();

	return Result;
}
bool CSerialPort::VWillDisConnect()
{
	return true;
}
bool CSerialPort::VDisConnect()
{
	bool Result = false;
	
	if(LogObject())
		LogObject()->Log(NOCOMMENTS, "Try to close %w", ComString());

	Result = (CloseHandle(m_hSerialPort) == TRUE);
	m_hSerialPort = INVALID_HANDLE_VALUE;
	
	return Result;
}
bool CSerialPort::VDidDisConnect()
{
	return true;
}
TU32_t CSerialPort::VReceive(TU08_t *pBuffer, TU32_t MaxSize)
{
	TU32_t Result = 0;
	
	if(Connected())
	{
		if(!ReadFile(m_hSerialPort,pBuffer,MaxSize,(LPDWORD)&Result,NULL))
		{
			if(LogObject())
				LogObject()->Log(ALERT, "Cannot Read on %w (error code : %u)",ComString(), GetLastError());
		}
		else
		{
			if(LogObject())
				LogObject()->Log(	NOCOMMENTS
									,"Read (%u/%u)On Port %w"
									, Result
									, MaxSize
									, ComString());
		}

		if(Result == 0)
			if(LogObject())
				LogObject()->Log(NOCOMMENTS, "No Data Read on %w (error code : %u)",ComString(), GetLastError());
	}

	return Result;
}
TU32_t CSerialPort::VSend(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier)
{	
	TU32_t Result = 0;
	
	if(Connected())
	{
		if(pSendBuffer && (Size > 0))
		{
			if(!WriteFile(m_hSerialPort,pSendBuffer,Size,(LPDWORD)&Result,NULL))
			{
				if(LogObject())
					LogObject()->Log(ALERT,"Cannot Write On Port %w (error code : %u)", ComString(), GetLastError());
			}
			else
			{
				if(LogObject())
					LogObject()->Log(	NOCOMMENTS
										,"Write (%u/%u)On Port %w"
										, Result
										, Size
										, ComString());
			}

			if(Result == 0)
				if(LogObject())
					LogObject()->Log(NOCOMMENTS, "No Data Sent on %w (error code : %u)",ComString(), GetLastError());
		}
	}
	return Result;
}
//void CSerialPort::Event_ConnectionStateChanged(BaseConnectedClientStatus_t NewConnectionState)
//{
//	if(m_pDelegate)
//		m_pDelegate->Event_ConnectionStateChanged(ConnectionState);
//}
//void CSerialPort::Event_FrameReceived(TU08_t * pBuffer, TU32_t Size, TU32_t SizeWaited)
//{
//	if(LogObject())
//		LogObject()->Log(NOCOMMENTS,(Size > 1 ? "Frame Received (%u/%u Bytes)":"Frame Received (%u/%u Byte)"), Size, SizeWaited);
//
//	if(m_pDelegate)
//		m_pDelegate->Event_FrameReceived(pBuffer, Size, SizeWaited);
//
//}
//void CSerialPort::Event_FrameSendResult(TU08_t * pBuffer, TU32_t BufferSize, TU32_t NBBytesSent, TU32_t FrameIDentifier)
//{
//	if(LogObject())
//		LogObject()->Log(NOCOMMENTS,((BufferSize == NBBytesSent) ? "Frame (ID : %u) send success" : "Frame (ID : %u) send failure"), FrameIDentifier);
//
//	if(m_pDelegate)
//		m_pDelegate->CBaseClient_Event_FrameSendResult((BufferSize == NBBytesSent), pBuffer, Size, FrameIDentifier);
//
//}