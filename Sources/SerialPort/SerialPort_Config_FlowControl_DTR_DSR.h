#ifndef CSERIALPORT_CONFIG_FLOWCONTROL_DTR_DSR_H
#define CSERIALPORT_CONFIG_FLOWCONTROL_DTR_DSR_H
#include "Common\MyTypes.h"
#include "SerialPort_Config_FlowControl.h"

class CSerialPort_Config_FlowControl_DTR_DSR : public CSerialPort_Config_FlowControl
{
public:
	CSerialPort_Config_FlowControl_DTR_DSR(	bool DSRSensitivity = false
								, SerialPort_Config_DTR_DSR_Control_t DTRControl = SerialPort_Config_DTR_DSR_Control_Enable
								, bool OutDSRFlow = false
								, TU16_t XOnLim = XOnLimDefaultValue
								, TU16_t XOffLim = XOffLimDefaultValue);
	CSerialPort_Config_FlowControl_DTR_DSR(CSerialPort_Config_FlowControl_DTR_DSR & Source);
	~CSerialPort_Config_FlowControl_DTR_DSR();

	bool SetNewFlowControl(CSerialPort_Config_FlowControl_DTR_DSR * pSource);

	inline bool DSRSensitivity(){return m_DSRSensitivity;}
	inline SerialPort_Config_DTR_DSR_Control_t DTRControl(){return m_DTRControl;}
	inline bool OutDSRFlow(){return m_OutDSRFlow;}
protected:
	bool SetNewFlowControl(CSerialPort_Config_FlowControl * pSource);
private:
	bool m_DSRSensitivity;
	SerialPort_Config_DTR_DSR_Control_t m_DTRControl;
	bool m_OutDSRFlow;
};
#endif