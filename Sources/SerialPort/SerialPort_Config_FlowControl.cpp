#include "SerialPort_Config_FlowControl.h"
CSerialPort_Config_FlowControl::CSerialPort_Config_FlowControl(SerialPort_Config_FlowControl_t FlowControl, TU16_t XOnLim, TU16_t XOffLim)
:m_FlowControl(FlowControl)
, m_XOnLim(XOnLim)
, m_XOffLim(XOffLim)
{
}
CSerialPort_Config_FlowControl::CSerialPort_Config_FlowControl(CSerialPort_Config_FlowControl & Source)
:m_FlowControl(Source.FlowControl())
, m_XOnLim(Source.XOnLim())
, m_XOffLim(Source.XOffLim())
{
}
CSerialPort_Config_FlowControl::~CSerialPort_Config_FlowControl()
{
}
bool CSerialPort_Config_FlowControl::SetNewFlowControl(CSerialPort_Config_FlowControl & Source)
{
	m_FlowControl = Source.FlowControl();
	m_XOnLim = Source.XOnLim();
	m_XOffLim = Source.XOffLim();
	return true;
}