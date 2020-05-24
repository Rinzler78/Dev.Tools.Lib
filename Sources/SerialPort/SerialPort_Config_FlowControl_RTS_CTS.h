#ifndef CSERIALPORT_CONFIG_FLOWCONTROL_RTS_CTS_H
#define CSERIALPORT_CONFIG_FLOWCONTROL_RTS_CTS_H
#include "Common\MyTypes.h"
#include "SerialPort_Config_FlowControl.h"

class CSerialPort_Config_FlowControl_RTS_CTS : public CSerialPort_Config_FlowControl
{
public:
	CSerialPort_Config_FlowControl_RTS_CTS(	bool CTSEnable = true
								, SerialPort_Config_RTS_CTS_Control_t RTSControl = SerialPort_Config_RTS_CTS_Control_HandShake
								, TU16_t XOnLim = XOnLimDefaultValue
								, TU16_t XOffLim = XOffLimDefaultValue);

	CSerialPort_Config_FlowControl_RTS_CTS(CSerialPort_Config_FlowControl_RTS_CTS & Source);
	~CSerialPort_Config_FlowControl_RTS_CTS();

	bool SetNewFlowControl(CSerialPort_Config_FlowControl_RTS_CTS * pSource);

	inline bool CTSEnable(){return m_CTSEnable;}
	inline SerialPort_Config_RTS_CTS_Control_t RTSControl(){return m_RTSControl;}
protected:
	bool SetNewFlowControl(CSerialPort_Config_FlowControl * pSource);
private:
	bool m_CTSEnable;
	SerialPort_Config_RTS_CTS_Control_t m_RTSControl;
};
#endif