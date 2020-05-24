#ifndef CBTDEVICE_H
#define CBTDEVICE_H
#define BT_NAME_SIZE		256
#define BT_PORT_NAME_SIZE	6

#include "CommonBluetooth\CommonBluetooth.h"
#include <iostream>
#include <list>

class CBTDevice
{
public:
	CBTDevice(const wchar_t * pDeviceName, BT_ADDR Device_Adrress = 0);
	~CBTDevice();
	bool CheckService(const unsigned short guid = SerialPortServiceClassID_UUID16);
	bool PairDevice(const char * pin_code, const char * MyName);
	bool UnpairDevice();
	bool GetBTDeviceInfo();
	inline bool Paired(){return m_Paired;}
	inline wchar_t * GetDeviceName(){return m_DeviceName;}
	inline BT_ADDR GetDeviceAdress(){return m_DeviceAdress;}
	inline BYTE GetChannel(){return m_Channel;}
	inline wchar_t * GetPortName(){return m_PortName;}
	inline wchar_t * GetPortPrefixe(){return m_PortPrefixe;}
	inline bool IsActive(){return m_IsActive;}
	inline wchar_t * GetFriendlyName(){return m_Friendlyname;}
	inline wchar_t * GetActiveKey(){return m_ActiveKey;}

protected:
	unsigned int GetBluetoothFreePort();
	bool ActivateBluetoothPort (unsigned int PortNumber);
	bool UpdateRegistryModemEntry();
	bool SaveDeviceInfo();
	wchar_t m_DeviceName[BT_NAME_SIZE]; ///< Name of the device
	BT_ADDR m_DeviceAdress; ///< The device adress
	BYTE m_Channel; ///< The channel
	HANDLE m_Handle;	///< Handle of the handle
	wchar_t	m_PortName [BT_PORT_NAME_SIZE]; ///< .
	wchar_t m_PortPrefixe [BT_PORT_NAME_SIZE];  ///< .
	bool m_IsActive;	///< true if this object is active
	wchar_t m_Friendlyname [BT_NAME_SIZE];  ///< .
	wchar_t m_ActiveKey [_MAX_PATH ];   ///< .
	bool m_Paired;  ///< true if paired
};

typedef std::list<CBTDevice> CBTDeviceList;
#endif