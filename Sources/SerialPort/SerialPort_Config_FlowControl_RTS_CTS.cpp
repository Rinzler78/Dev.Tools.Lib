#include "SerialPort_Config_FlowControl_RTS_CTS.h"
CSerialPort_Config_FlowControl_RTS_CTS::CSerialPort_Config_FlowControl_RTS_CTS(
	bool CTSEnable, SerialPort_Config_RTS_CTS_Control_t RTSControl, TU16_t XOnLim, TU16_t XOffLim)
:CSerialPort_Config_FlowControl(SerialPort_Config_FlowControl_RTS_CTS, XOnLim, XOffLim)
, m_CTSEnable(CTSEnable)
, m_RTSControl(RTSControl)
{
}
CSerialPort_Config_FlowControl_RTS_CTS::CSerialPort_Config_FlowControl_RTS_CTS(CSerialPort_Config_FlowControl_RTS_CTS & Source)
:CSerialPort_Config_FlowControl(Source)
, m_CTSEnable(Source.CTSEnable())
, m_RTSControl(Source.RTSControl())
{
}
CSerialPort_Config_FlowControl_RTS_CTS::~CSerialPort_Config_FlowControl_RTS_CTS()
{
}
bool CSerialPort_Config_FlowControl_RTS_CTS::SetNewFlowControl(CSerialPort_Config_FlowControl_RTS_CTS * pSource)
{
	return CSerialPort_Config_FlowControl_RTS_CTS::SetNewFlowControl((CSerialPort_Config_FlowControl *)pSource);
}
bool CSerialPort_Config_FlowControl_RTS_CTS::SetNewFlowControl(CSerialPort_Config_FlowControl * pSource)
{
	bool Result = false;

	if(pSource)
	{
		m_CTSEnable = ((CSerialPort_Config_FlowControl_RTS_CTS *)pSource)->CTSEnable();
		m_RTSControl = ((CSerialPort_Config_FlowControl_RTS_CTS *)pSource)->RTSControl();
		Result = CSerialPort_Config_FlowControl::SetNewFlowControl(*pSource);
	}

	return Result;
}