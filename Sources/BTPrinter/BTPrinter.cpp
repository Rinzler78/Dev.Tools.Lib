#include "BTPrinter.h"
#include "BTManager.h"
CBTPrinter::CBTPrinter(const wchar_t * DeviceName, BT_ADDR Device_Adrress)
: CBTDevice(DeviceName, Device_Adrress)
{
	
}
CBTPrinter::~CBTPrinter()
{
}
bool CBTPrinter::Activate()
{
	bool Result = false;
	unsigned int port_index = 0;
	HKEY hk = NULL;

	/** Add prefixe Port */
	wcscpy(m_PortPrefixe,DEFAULT_PREFIXE_PORT);

	if(ActivateBluetoothPort(GetBluetoothFreePort())) 
	{
		if (ERROR_SUCCESS != RegOpenKeyEx (HKEY_LOCAL_MACHINE, L"Printers\\Ports", 0, KEY_ALL_ACCESS, &hk)) 
		{
			//btDev.isActive = FALSE;

			DeactivateDevice (m_Handle);
			m_Handle = NULL;

			return FALSE;
		}

		//add the COM port mapping into the Printer\Ports key so that other applications can pick it up
		int iMinPort = 10;
		WCHAR szPortName[10];
		for (int i = 1; i < 10 ; ++i) {
			wsprintf (szPortName, L"Port%d", i);
			WCHAR szString[_MAX_PATH];
			DWORD dwType;
			DWORD dwSize = sizeof(szString);
			if (ERROR_SUCCESS == RegQueryValueEx (hk, szPortName, NULL, &dwType, (BYTE *)szString, &dwSize)) {
				if ((dwType == REG_SZ) && (wcsicmp (szString, m_PortName) == 0)) {
					RegCloseKey (hk);
					return TRUE;
				}
			} else if (iMinPort > i)
				iMinPort = i;
		}

		if (iMinPort < 10) {
			wsprintf (szPortName, L"Port%d", iMinPort);
			RegSetValueEx (hk, szPortName, 0, REG_SZ, (BYTE *)m_PortName, (wcslen (m_PortName) + 1) * sizeof(WCHAR));
		} else
			m_IsActive = FALSE;

		RegCloseKey (hk);

		RegFlushKey (HKEY_LOCAL_MACHINE);
	}

	return Result;
}
bool CBTPrinter::Deactivate()
{
	bool Result = false;
	
	TCHAR key [_MAX_PATH ];
	HKEY hk;
	LONG RetReg = 0;
	BOOL bRet;

	// Check if handle was initialised in current BTPrinter run-time.
	if ( m_Friendlyname[0] == '\0' ) {
		// Try to get handle from registry if it exists.
		StrTCbPrintf ( key, sizeof ( key ), _T("software\\microsoft\\bluetooth\\device\\printer\\%04x%08x"), GET_NAP(m_DeviceAdress), GET_SAP(m_DeviceAdress));
		WCHAR szString[256];
		DWORD dwSize = sizeof(szString);
		DWORD dwType;
		if (ERROR_SUCCESS == RegOpenKeyEx (HKEY_LOCAL_MACHINE, key, 0, KEY_ALL_ACCESS, &hk))
		{
			if (ERROR_SUCCESS == RegQueryValueEx (hk, _T("name"), NULL, &dwType, (BYTE *)szString, &dwSize)) {
				if ((dwType == REG_SZ)) {
					_tcscpy_s(m_Friendlyname, sizeof(m_Friendlyname), szString);
					RegCloseKey(hk);
				}
			}
		}
	}

	if ( _tcscmp(m_PortName, DEFAULT_DEVICENAME) == 0 ) {
		// Try to get handle from registry if it exists.
		StrTCbPrintf ( key, sizeof ( key ), _T("software\\microsoft\\bluetooth\\device\\printer\\%04x%08x"), GET_NAP(m_DeviceAdress), GET_SAP(m_DeviceAdress));
		WCHAR szString[256];
		DWORD dwSize = sizeof(szString);
		DWORD dwType;
		if (ERROR_SUCCESS == RegOpenKeyEx (HKEY_LOCAL_MACHINE, key, 0, KEY_ALL_ACCESS, &hk))
		{
			if (ERROR_SUCCESS == RegQueryValueEx (hk, _T("port_name"), NULL, &dwType, (BYTE *)szString, &dwSize)) {
				if ((dwType == REG_SZ)) {
					_tcscpy_s(m_PortName, sizeof(m_PortName), szString);
					RegCloseKey(hk);
				}
			}
		}
	}

	if (m_Handle == NULL ) {
		// Try to get handle from registry if it exists.
		DWORD dwHandle = 0;
		DWORD dwSize = sizeof(dwHandle);
		DWORD dwType;
		if (ERROR_SUCCESS == RegOpenKeyEx (HKEY_LOCAL_MACHINE, m_ActiveKey, 0, KEY_ALL_ACCESS, &hk))
		{
			if (ERROR_SUCCESS == RegQueryValueEx (hk, _T("Hnd"), NULL, &dwType, (BYTE *)&dwHandle, &dwSize)) {
				if ((dwType == REG_DWORD)) {
					m_Handle = (HANDLE)dwHandle;
					RegCloseKey(hk);
				}
			}
		}
	}

	if ( m_Handle != NULL ) 
	{	// Handle is valid. Deactivate the device.
		bRet = DeactivateDevice (m_Handle);
		Sleep ( 500 ); // Wait for deactivation
		if(!bRet) {
			// Indicate failure.
			return false;
		}
	}

	// undo the whole stuff just in the reverse order.

	StrTCbPrintf ( key, sizeof ( key ), _T("software\\microsoft\\bluetooth\\device\\ports\\%s"), m_Friendlyname);
	RetReg = RegDeleteKey ( HKEY_LOCAL_MACHINE, key );

	StrTCbPrintf ( key, sizeof ( key ), _T("software\\microsoft\\bluetooth\\device\\printer\\%04x%08x"), GET_NAP(m_DeviceAdress), GET_SAP(m_DeviceAdress));
	RetReg = RegDeleteKey   ( HKEY_LOCAL_MACHINE, key );

	if (ERROR_SUCCESS != RegOpenKeyEx (HKEY_LOCAL_MACHINE, L"Printers\\Ports", 0, KEY_ALL_ACCESS, &hk))
		return FALSE;

	for (int i = 1; i < 10 ; ++i) {
		WCHAR szPortName[10];
		StrTCchPrintf (STRING_AND_COUNTOF(szPortName), L"Port%d", i);

		WCHAR szString[256];
		DWORD dwSize = sizeof(szString);
		DWORD dwType;
		if (ERROR_SUCCESS == RegQueryValueEx (hk, szPortName, NULL, &dwType, (BYTE *)szString, &dwSize)) {
			if ((dwType == REG_SZ) && (wcsicmp (szString, m_PortName) == 0)) {
				RegDeleteValue (hk, szPortName);
				break;
			}
		}
	}

	RegCloseKey (hk);
	m_IsActive = false;
	m_Handle   = NULL;
	memset ( m_PortName, 0, BT_PORT_NAME_SIZE * sizeof ( TCHAR ) );

	return Result;
}
bool Write(const char * TexteToPrint)
{
	bool Result = false;

	return Result;
}