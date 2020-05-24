#include "SerialPort_Config_FlowControl_DTR_DSR.h"
CSerialPort_Config_FlowControl_DTR_DSR::CSerialPort_Config_FlowControl_DTR_DSR(
	bool DSRSensitivity, SerialPort_Config_DTR_DSR_Control_t DTRControl, bool OutDSRFlow, TU16_t XOnLim, TU16_t XOffLim)
:CSerialPort_Config_FlowControl(SerialPort_Config_FlowControl_DTR_DSR, XOnLim, XOffLim)
, m_DSRSensitivity(DSRSensitivity)
, m_DTRControl(DTRControl)
, m_OutDSRFlow(OutDSRFlow)
{
}
CSerialPort_Config_FlowControl_DTR_DSR::CSerialPort_Config_FlowControl_DTR_DSR(CSerialPort_Config_FlowControl_DTR_DSR & Source)
:CSerialPort_Config_FlowControl(Source.FlowControl(), Source.XOnLim(), Source.XOnLim())
, m_DSRSensitivity(Source.DSRSensitivity())
, m_DTRControl(Source.DTRControl())
, m_OutDSRFlow(Source.OutDSRFlow())
{
}
CSerialPort_Config_FlowControl_DTR_DSR::~CSerialPort_Config_FlowControl_DTR_DSR()
{
}
bool CSerialPort_Config_FlowControl_DTR_DSR::SetNewFlowControl(CSerialPort_Config_FlowControl_DTR_DSR * pSource)
{
	return CSerialPort_Config_FlowControl_DTR_DSR::SetNewFlowControl((CSerialPort_Config_FlowControl *)pSource);
}
bool CSerialPort_Config_FlowControl_DTR_DSR::SetNewFlowControl(CSerialPort_Config_FlowControl * pSource)
{
	bool Result = false;

	if(pSource)
	{
		m_DSRSensitivity = ((CSerialPort_Config_FlowControl_DTR_DSR *)pSource)->DSRSensitivity();
		m_DTRControl = ((CSerialPort_Config_FlowControl_DTR_DSR *)pSource)->DTRControl();
		m_OutDSRFlow = ((CSerialPort_Config_FlowControl_DTR_DSR *)pSource)->OutDSRFlow();
		Result = CSerialPort_Config_FlowControl::SetNewFlowControl(*pSource);
	}
	return Result;
}