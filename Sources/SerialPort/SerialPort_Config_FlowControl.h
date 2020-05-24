#ifndef CSERIALPORT_CONFIG_FLOWCONTROL_H
#define CSERIALPORT_CONFIG_FLOWCONTROL_H
#include "Common\MyTypes.h"
#include "SerialPort\SerialPort_Config.h"
/** Default value */
static const TU16_t XOnLimDefaultValue = 2048;
static const TU16_t XOffLimDefaultValue = 2048;

class CSerialPort_Config_FlowControl
{
public:
	CSerialPort_Config_FlowControl(	SerialPort_Config_FlowControl_t FlowControl
									, TU16_t XOnLim = XOnLimDefaultValue
									, TU16_t XOffLim = XOffLimDefaultValue);
	CSerialPort_Config_FlowControl(CSerialPort_Config_FlowControl & Source);
	~CSerialPort_Config_FlowControl();
	
	inline SerialPort_Config_FlowControl_t FlowControl(){return m_FlowControl;};
	inline TU16_t XOnLim(){return m_XOnLim;}
	inline TU16_t XOffLim(){return m_XOffLim;}
	
protected:
	virtual bool SetNewFlowControl(CSerialPort_Config_FlowControl * pSource) = NULL;
	bool SetNewFlowControl(CSerialPort_Config_FlowControl & Source);
private:
	SerialPort_Config_FlowControl_t m_FlowControl;
	TU16_t m_XOnLim;
	TU16_t m_XOffLim;
};
#endif