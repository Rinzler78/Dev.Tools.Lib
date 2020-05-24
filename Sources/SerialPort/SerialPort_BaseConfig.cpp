#include "SerialPort\SerialPort_BaseConfig.h"
#include "SerialPort_Config_FlowControl_XOnXOff.h"
#include "SerialPort_CONFIG_FLOWCONTROL_DTR_DSR.h"
#include "SerialPort_Config_FlowControl_RTS_CTS.h"
#include "String.h"
#include "windows.h"
CSerialPort_BaseConfig::CSerialPort_BaseConfig(	SerialPort_Config_Baudrate_t BaudRate
							, SerialPort_Config_Parity_t Parity
							, SerialPort_Config_DataBits_t NbDataBits
							, SerialPort_Config_StopBits_t NbStopBits
							, CSerialPort_Config_FlowControl * pFlowControl
							, TC08_t EventStartChar
							, TC08_t EventStopChar
							, TC08_t ErrorChar
							, bool ReplaceParityErrorByErrorChar
							, bool DiscardNULLChar)
:m_BaudRate(BaudRate)
, m_Parity(Parity)
, m_NbDataBits(NbDataBits)
, m_NbStopBits(NbStopBits)
, m_pFlowControl(NULL)
, m_EventStartChar(EventStartChar)
, m_EventStopChar(EventStopChar)
, m_ErrorChar(ErrorChar)
, m_ReplaceParityErrorByErrorChar(ReplaceParityErrorByErrorChar)
, m_DiscardNULLChar(DiscardNULLChar)
{
	/** Flow Control */
	GetFlowControl(pFlowControl);
}
CSerialPort_BaseConfig::CSerialPort_BaseConfig(CSerialPort_BaseConfig & Source)
:m_BaudRate(Source.BaudRate())
, m_Parity(Source.Parity())
, m_NbDataBits(Source.DataBits())
, m_NbStopBits(Source.StopBits())
, m_pFlowControl(NULL)
, m_EventStartChar(Source.EventStartChar())
, m_EventStopChar(Source.EventStopChar())
, m_ErrorChar(Source.ErrorChar())
, m_ReplaceParityErrorByErrorChar(Source.ReplaceParityErrorByErrorChar())
, m_DiscardNULLChar(Source.DiscardNULLChar())
{
	GetFlowControl(Source.FlowControl());
}
CSerialPort_BaseConfig::~CSerialPort_BaseConfig()
{
	DeleteFlowControl(m_pFlowControl);
}
bool CSerialPort_BaseConfig::SetNewConfiguration(CSerialPort_BaseConfig & Source)
{
	bool Result = true;
	
	m_BaudRate = Source.BaudRate();
	m_Parity = Source.Parity();
	m_NbDataBits = Source.DataBits();
	m_NbStopBits = Source.StopBits();
	
	MACRO_DELET(m_pFlowControl)

	if(Source.FlowControl())
	{
		switch(Source.FlowControl()->FlowControl())
		{
			case SerialPort_Config_FlowControl_XOn_XOff:
				m_pFlowControl = new CSerialPort_Config_FlowControl_XOnXOff((CSerialPort_Config_FlowControl_XOnXOff &) *Source.FlowControl());
				break;
			case SerialPort_Config_FlowControl_DTR_DSR:
				m_pFlowControl = new CSerialPort_Config_FlowControl_DTR_DSR((CSerialPort_Config_FlowControl_DTR_DSR &) *Source.FlowControl());
				break;
			case SerialPort_Config_FlowControl_RTS_CTS:
				m_pFlowControl = new CSerialPort_Config_FlowControl_RTS_CTS((CSerialPort_Config_FlowControl_RTS_CTS &) *Source.FlowControl());
				break;
			default:
				break;
		}
	}

	m_EventStartChar = Source.EventStartChar();
	m_EventStopChar = Source.EventStopChar();
	m_ErrorChar = Source.ErrorChar();
	m_ReplaceParityErrorByErrorChar = Source.ReplaceParityErrorByErrorChar();
	m_DiscardNULLChar = Source.DiscardNULLChar();

	return Result;

}
bool CSerialPort_BaseConfig::GetFlowControl(CSerialPort_Config_FlowControl * pFlowControl)
{
	bool Result = false;
	
	if(pFlowControl)
	{
		Result = true;
		switch(pFlowControl->FlowControl())
		{
			case SerialPort_Config_FlowControl_DTR_DSR:
				m_pFlowControl = new CSerialPort_Config_FlowControl_DTR_DSR((CSerialPort_Config_FlowControl_DTR_DSR &)*pFlowControl);
				break;
			case SerialPort_Config_FlowControl_XOn_XOff:
				m_pFlowControl = new CSerialPort_Config_FlowControl_XOnXOff((CSerialPort_Config_FlowControl_XOnXOff &)*pFlowControl);
				break;
			case SerialPort_Config_FlowControl_RTS_CTS:
				m_pFlowControl = new CSerialPort_Config_FlowControl_RTS_CTS((CSerialPort_Config_FlowControl_RTS_CTS &)*pFlowControl);
				break;
			default:
				Result = false;
				break;
		}
	}

	return Result;
}
bool CSerialPort_BaseConfig::DeleteFlowControl(CSerialPort_Config_FlowControl * pFlowControl)
{
	bool Result = false;
	
	if(pFlowControl)
	{
		Result = true;
		switch(pFlowControl->FlowControl())
		{
			case SerialPort_Config_FlowControl_None:
				MACRO_DELET(m_pFlowControl)
				break;
			case SerialPort_Config_FlowControl_DTR_DSR:
				{
					CSerialPort_Config_FlowControl_DTR_DSR * pFlow = (CSerialPort_Config_FlowControl_DTR_DSR *) pFlowControl;
					MACRO_DELET(pFlow)
				}
				break;
			case SerialPort_Config_FlowControl_XOn_XOff:
				{
					CSerialPort_Config_FlowControl_XOnXOff * pFlow = (CSerialPort_Config_FlowControl_XOnXOff *) pFlowControl;
					MACRO_DELET(pFlow)
				}
				break;
			case SerialPort_Config_FlowControl_RTS_CTS:
				{
					CSerialPort_Config_FlowControl_RTS_CTS * pFlow = (CSerialPort_Config_FlowControl_RTS_CTS *) pFlowControl;
					MACRO_DELET(pFlow)
				}
				break;
			default:
				Result = false;
				break;
		}
	}

	return Result;
}
