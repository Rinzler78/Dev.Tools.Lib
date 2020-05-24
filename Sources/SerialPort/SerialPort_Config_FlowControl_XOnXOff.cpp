#include "SerialPort_Config_FlowControl_XOnXOff.h"
CSerialPort_Config_FlowControl_XOnXOff::CSerialPort_Config_FlowControl_XOnXOff(
	TC08_t XOnChar, TC08_t XOffChar, bool ContinueAfterXOff, TU16_t XOnLim, TU16_t XOffLim)
	:CSerialPort_Config_FlowControl(SerialPort_Config_FlowControl_XOn_XOff, XOnLim, XOffLim)
, m_XOnChar(XOnChar)
, m_XOffChar(XOffChar)
, m_ContinueAfterXOff(ContinueAfterXOff)
{
}
CSerialPort_Config_FlowControl_XOnXOff::CSerialPort_Config_FlowControl_XOnXOff(CSerialPort_Config_FlowControl_XOnXOff & Source)
:CSerialPort_Config_FlowControl(Source)
, m_XOnChar(Source.XOnChar())
, m_XOffChar(Source.XOffChar())
, m_ContinueAfterXOff(Source.ContinueAfterXOff())
{

}
CSerialPort_Config_FlowControl_XOnXOff::~CSerialPort_Config_FlowControl_XOnXOff()
{
}
bool CSerialPort_Config_FlowControl_XOnXOff::SetNewFlowControl(CSerialPort_Config_FlowControl_XOnXOff * pSource)
{
	return CSerialPort_Config_FlowControl_XOnXOff::SetNewFlowControl((CSerialPort_Config_FlowControl *)pSource);
}
bool CSerialPort_Config_FlowControl_XOnXOff::SetNewFlowControl(CSerialPort_Config_FlowControl * pSource)
{
	bool Result = false;

	if(pSource)
	{
		m_XOnChar = ((CSerialPort_Config_FlowControl_XOnXOff *)pSource)->XOnChar();
		m_XOffChar = ((CSerialPort_Config_FlowControl_XOnXOff *)pSource)->XOffChar();
		m_ContinueAfterXOff = ((CSerialPort_Config_FlowControl_XOnXOff *)pSource)->ContinueAfterXOff();
		Result = CSerialPort_Config_FlowControl::SetNewFlowControl(*pSource);
	}
	return Result;
}