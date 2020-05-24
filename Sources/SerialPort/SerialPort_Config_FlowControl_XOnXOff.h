#ifndef CSERIALPORT_XONXOFFCONFIG_H
#define CSERIALPORT_XONXOFFCONFIG_H
#include "Common\MyTypes.h"
#include "SerialPort_Config_FlowControl.h"

static const TC08_t XOnDefaultChar = 17;
static const TC08_t XOffDefaultChar = 18;

class CSerialPort_Config_FlowControl_XOnXOff : public CSerialPort_Config_FlowControl
{
public:
	CSerialPort_Config_FlowControl_XOnXOff(	TC08_t XOnChar = XOnDefaultChar
											, TC08_t XOffChar = XOffDefaultChar
											, bool ContinueAfterXOff = false
											, TU16_t XOnLim = XOnLimDefaultValue
											, TU16_t XOffLim = XOffLimDefaultValue);

	CSerialPort_Config_FlowControl_XOnXOff(CSerialPort_Config_FlowControl_XOnXOff & Source);
	~CSerialPort_Config_FlowControl_XOnXOff();

	bool SetNewFlowControl(CSerialPort_Config_FlowControl_XOnXOff * pSource);

	inline TC08_t XOnChar(){return m_XOnChar;};
	inline TC08_t XOffChar(){return m_XOffChar;};
	inline bool ContinueAfterXOff(){return m_ContinueAfterXOff;}
protected:
	bool SetNewFlowControl(CSerialPort_Config_FlowControl * pSource);
private:
	TC08_t m_XOnChar;
	TC08_t m_XOffChar;
	bool m_ContinueAfterXOff;
};
#endif