#ifndef CSERIALPORT_BASECONFIG_H
#define CSERIALPORT_BASECONFIG_H
#include "Common\MyTypes.h"
#include "SerialPort\SerialPort_Config.h"

/** Default Value */
/** Com Event */
static const TC08_t Default_EventStartChar = 0;
static const TC08_t Default_EventStopChar = 0;
static const TC08_t Default_ErrorChar = 0;
static const bool Default_ReplaceParityErrorByErrorChar = false;
static const bool Default_DiscardNULLChar = false;

class CSerialPort_Config_FlowControl;

class CSerialPort_BaseConfig
{
public:
	CSerialPort_BaseConfig(SerialPort_Config_Baudrate_t BaudRate = SerialPort_Config_Baudrate_9600
							, SerialPort_Config_Parity_t Parity = SerialPort_Config_Parity_None
							, SerialPort_Config_DataBits_t NbDataBits = SerialPort_Config_DataBits_8
							, SerialPort_Config_StopBits_t NbStopBits = SerialPort_Config_StopBits_1
							, CSerialPort_Config_FlowControl * pFlowControl = NULL
							, TC08_t EventStartChar = Default_EventStartChar
							, TC08_t EventStopChar = Default_EventStopChar
							, TC08_t ErrorChar = Default_ErrorChar
							, bool ReplaceParityErrorByErrorChar = Default_ReplaceParityErrorByErrorChar
							, bool DiscardNULLChar = Default_DiscardNULLChar);
	CSerialPort_BaseConfig(CSerialPort_BaseConfig & Source);
	~CSerialPort_BaseConfig();

	bool SetNewConfiguration(CSerialPort_BaseConfig & Source);
	/** Baud Rate */
	inline SerialPort_Config_Baudrate_t BaudRate(){return m_BaudRate;}
	/** Parity */
	inline SerialPort_Config_Parity_t Parity(){return m_Parity;}
	/** Data Bits */
	inline SerialPort_Config_DataBits_t DataBits(){return m_NbDataBits;}
	/** Stop Bits */
	inline SerialPort_Config_StopBits_t StopBits(){return m_NbStopBits;}
	/** Flow Control */
	inline CSerialPort_Config_FlowControl * FlowControl() {return m_pFlowControl;}
	/** Com Event */
	inline TC08_t EventStartChar(){return m_EventStartChar;};
	inline TC08_t EventStopChar(){return m_EventStopChar;};

	/** Error */
	inline TC08_t ErrorChar(){return m_ErrorChar;};
	inline bool ReplaceParityErrorByErrorChar(){return m_ReplaceParityErrorByErrorChar;};
	inline bool DiscardNULLChar(){return m_DiscardNULLChar;};
private:
	/** Baud Rate */
	SerialPort_Config_Baudrate_t m_BaudRate;
	/** Parity */
	SerialPort_Config_Parity_t m_Parity;
	/** Data Bits */
	SerialPort_Config_DataBits_t m_NbDataBits;
	/** Stop Bits */
	SerialPort_Config_StopBits_t m_NbStopBits;

	/** Flow Control */
	CSerialPort_Config_FlowControl * m_pFlowControl;
	bool GetFlowControl(CSerialPort_Config_FlowControl * pFlowControl);
	bool DeleteFlowControl(CSerialPort_Config_FlowControl * pFlowControl);
	/** Com Event */
	TC08_t m_EventStartChar;
	TC08_t m_EventStopChar;

	/** Error */
	TC08_t m_ErrorChar;
	bool m_ReplaceParityErrorByErrorChar;
	bool m_DiscardNULLChar;
};
#endif