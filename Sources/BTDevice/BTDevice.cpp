#include "BTDevice\BTDevice.h"
#include "BTManager.h"
#include "stdio.h"
#include "bt_sdp.h"

CBTDevice::CBTDevice(const wchar_t * DeviceName, BT_ADDR Device_Adrress)
: m_DeviceAdress(0)
, m_Channel(0)
, m_Handle(NULL)
, m_IsActive(false)
, m_Paired(false)
{
	/** Init Wchar_t Tabs */
	memset(m_DeviceName, 0, BT_NAME_SIZE * sizeof(wchar_t));
	memset(m_PortName, 0, BT_PORT_NAME_SIZE * sizeof(wchar_t));
	memset(m_PortPrefixe, 0, BT_PORT_NAME_SIZE * sizeof(wchar_t));
	memset(m_Friendlyname, 0, BT_NAME_SIZE * sizeof(wchar_t));
	memset(m_ActiveKey, 0, _MAX_PATH * sizeof(wchar_t));

	/** Complete Device infos */
	if(DeviceName)
		wcscpy(m_DeviceName, DeviceName);
	m_DeviceAdress = Device_Adrress;
}
CBTDevice::~CBTDevice()
{
}

bool CBTDevice::CheckService(const unsigned short guid)
{
	CoInitializeEx (0, COINIT_MULTITHREADED);
	bool Result = false;
	BTHNS_RESTRICTIONBLOB RBlob = {0};
	BLOB blob = {0};
	SOCKADDR_BTH sa = {0};
	CSADDR_INFO csai = {0};
	WSAQUERYSET wsaq  = {0};
	HANDLE hLookup = NULL; 
	
	/** Rblob value ? */
	RBlob.type = SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST;
	RBlob.numRange               = 1;
	RBlob.pRange[0].minAttribute = SDP_ATTRIB_PROTOCOL_DESCRIPTOR_LIST;
	RBlob.pRange[0].maxAttribute = SDP_ATTRIB_PROTOCOL_DESCRIPTOR_LIST;
	RBlob.uuids[0].uuidType      = SDP_ST_UUID16;
	RBlob.uuids[0].u.uuid16      = guid;
	
	/** blob value ? */
	blob.cbSize = sizeof(RBlob);
	blob.pBlobData = (BYTE *)&RBlob;
	
	/** Query Adresse */
	sa.btAddr = m_DeviceAdress;
	sa.addressFamily         = AF_BT;
	
	/** csai value ? */
	csai.RemoteAddr.lpSockaddr = (sockaddr *)&sa;
	csai.RemoteAddr.iSockaddrLength = sizeof(sa);
	
	/** wsaq value ? */
	wsaq.dwSize      = sizeof(wsaq);
	wsaq.dwNameSpace = NS_BTH;
	wsaq.lpBlob      = &blob;
	wsaq.lpcsaBuffer = &csai;

	int iRet = BthNsLookupServiceBegin (&wsaq, 0, &hLookup);

	if (ERROR_SUCCESS == iRet)
	{
		union {
			CHAR buf[5000];
			SOCKADDR_BTH	__unused;	// properly align buffer to BT_ADDR requirements
		};
		

		LPWSAQUERYSET pwsaResults = (LPWSAQUERYSET) buf;

		DWORD dwSize  = sizeof(buf);

		memset(pwsaResults,0,sizeof(WSAQUERYSET));
		pwsaResults->dwSize      = sizeof(WSAQUERYSET);
		pwsaResults->dwNameSpace = NS_BTH;
		pwsaResults->lpBlob      = NULL;

		iRet = BthNsLookupServiceNext (hLookup, 0, &dwSize, pwsaResults);
		if (iRet == ERROR_SUCCESS) {	// Success - got the stream
			unsigned char cChannel = 0;
			if (ERROR_SUCCESS == CBTManager::FindRFCOMMChannel (	pwsaResults->lpBlob->pBlobData,
														pwsaResults->lpBlob->cbSize,
														&cChannel))
			{
				m_Channel = cChannel;
				Result = true;
			}
		}

		BthNsLookupServiceEnd(hLookup);
	}

	CoUninitialize ();

	return Result;
}
bool CBTDevice::PairDevice	(const char * pin_code, const char * MyName)
{
	// Create local variable to do 5 calls to BthPairRequest (DeviceIoControl with BT_IOCTL_BthPairRequest fails many times).
	// When retry is 0, assume Device can't be paired.
	int retry = 20; /** Number of retry */
	int ret_val = -1;
	bool Result = false;
	BYTE Size = 0;

	/** Unpair if it is */
	UnpairDevice();

	// The API tells us that we do not count the 0 at the end of the pin code ...
	while(retry && ( ret_val != ERROR_SUCCESS)) 
	{
		// mandatory
		Sleep ( 1500 );

		ret_val = BthPairRequest ((BT_ADDR *)&m_DeviceAdress, strlen(pin_code), (unsigned char*) pin_code );
		retry--;
	}

	if ( ret_val == ERROR_SUCCESS)
	{
		if ( BthSetLinkKey ( (BT_ADDR*)&m_DeviceAdress, (unsigned char *)MyName ) == ERROR_SUCCESS )
			Result = m_Paired = true;
	}

	return Result;
}
bool CBTDevice::UnpairDevice()
{
	bool Result = false;

	if ( BthRevokePIN  ( (BT_ADDR*)&m_DeviceAdress) == ERROR_SUCCESS)
	{
		if ( BthRevokeLinkKey ( (BT_ADDR*)&m_DeviceAdress ) == ERROR_SUCCESS )
		{
			Result = m_Paired = true;
		}
	}

	return Result;
}
bool CBTDevice::GetBTDeviceInfo()
{
	bool Result = false;
	
	DEVMGR_DEVICE_INFORMATION DevInfo;
	HANDLE hDev = INVALID_HANDLE_VALUE;
	_TCHAR cSearchParam[BT_PORT_NAME_SIZE] = {0};
	bool bFound = FALSE;
	
	memset(&DevInfo, 0, sizeof(DEVMGR_DEVICE_INFORMATION));
	DevInfo.dwSize = sizeof(DEVMGR_DEVICE_INFORMATION);
	_tcscpy_s(cSearchParam, sizeof(cSearchParam), m_PortPrefixe);
	_tcscat_s(cSearchParam, sizeof(cSearchParam), _T("*"));

	hDev = FindFirstDevice(DeviceSearchByLegacyName, cSearchParam ,&DevInfo);
	if(hDev != INVALID_HANDLE_VALUE) 
	{
		do {
			bFound = !_tcscmp(DevInfo.szLegacyName, m_PortName);
			if(!bFound && !FindNextDevice(hDev, &DevInfo)) {
				break;
			}

		} while(!bFound);
		if(bFound) {
			/** Reset Active Key */
			memset(m_ActiveKey,0, sizeof(m_ActiveKey) * sizeof(*m_ActiveKey));
			wcscpy(m_ActiveKey, DevInfo.szDeviceKey);

			/** Reset Handle */
			m_Handle = DevInfo.hDevice;
		
			Result = bFound;
		}
	}

	return Result;
}
unsigned int CBTDevice::GetBluetoothFreePort ()
{
	unsigned int Result = 0;
	TCHAR szPortName[20] = {0};

	if(Result == 0)
	{
		for (unsigned int i = 2 ; i < 10 ; ++i)
		{
			wsprintf(szPortName, _T("%s%d:"), m_PortPrefixe, i);

			HANDLE hFile = CreateFile (szPortName, 0, 0, NULL, OPEN_EXISTING, 0, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				Result = i;
				
				break;
			}
			CloseHandle (hFile);
			memset(szPortName,0,sizeof(szPortName) * sizeof(*szPortName));
		}
	}

	return Result;
}
bool CBTDevice::ActivateBluetoothPort (unsigned int PortNumber)
{
	bool Result = false;
	TCHAR key [_MAX_PATH ] = {0}; 
	TCHAR friendlyname [_MAX_PATH] = {0}; 
	DWORD dwDisp = 0;
	DWORD dwValue;
	PORTEMUPortParams pp = {0}; 
	HKEY hkModem = NULL;

	wsprintf(m_PortName, _T("%s%d:"), m_PortPrefixe, PortNumber);

	// Build and store the friendly name
	if(m_Friendlyname[0] == '\0') 
	{
		StrTCbPrintf (	m_Friendlyname
							, sizeof(m_Friendlyname) * sizeof(*m_Friendlyname)
							, _T("%s (%04x%08x)")
							, m_DeviceName
							, GET_NAP(m_DeviceAdress)
							, GET_SAP(m_DeviceAdress));
	}

	// Now create the modem entry
	// We will finish to fill it up later
	
	memset (&pp, 0, sizeof(pp));
	memcpy ( &pp.device, &m_DeviceAdress, sizeof ( BT_ADDR ) );
	pp.channel      = m_Channel;
	pp.imtu         = 0x800;
	pp.uiportflags |= RFCOMM_PORT_FLAGS_AUTHENTICATE | RFCOMM_PORT_FLAGS_REMOTE_DCB;
	pp.flocal       = 0; // We are client
	// Don't fill this one as the protocol will RE-search for the proper channel corresponding to thaht GUIF.
	// We have already done it and pass it through pp.channel
	pp.uuidService  = SerialPortServiceClass_UUID;
	
	StrTCbPrintf ( key, sizeof ( key ), _T("software\\microsoft\\bluetooth\\device\\printer\\%04x%08x"), GET_NAP(m_DeviceAdress), GET_SAP(m_DeviceAdress));
	if (ERROR_SUCCESS == RegCreateKeyEx (HKEY_LOCAL_MACHINE, key, 0, NULL, 0, KEY_WRITE, NULL, &hkModem, &dwDisp))
	{
		DWORD zero_value = 0;
		DWORD dwChannel = pp.channel;
		// fill the parameters
		RegSetValueEx (hkModem, _T("name")		  , 0, REG_SZ   , (BYTE *)m_Friendlyname, (_tcslen (m_Friendlyname) + 1) * sizeof(_TCHAR));
		RegSetValueEx (hkModem, _T("handle")      , 0, REG_DWORD, (BYTE *)&zero_value       , sizeof(DWORD));
		RegSetValueEx (hkModem, _T("active")      , 0, REG_DWORD, (BYTE *)&zero_value       , sizeof(DWORD));
		RegSetValueEx (hkModem, _T("port_name")   , 0, REG_SZ   , (BYTE *)m_PortName   , (_tcslen (m_PortName) + 1) * sizeof(_TCHAR));
		RegSetValueEx (hkModem, _T("scanHandle")  , 0, REG_DWORD, (BYTE *)&zero_value       , sizeof(DWORD));
		RegSetValueEx (hkModem, _T("encrypt")     , 0, REG_DWORD, (BYTE *)&zero_value       , sizeof(DWORD));
		dwValue = 0; // Do not authenticate.
		RegSetValueEx (hkModem, _T("auth")        , 0, REG_DWORD, (BYTE *)&dwValue          , sizeof(DWORD));
		RegSetValueEx (hkModem, _T("outgoing")    , 0, REG_DWORD, (BYTE *)&zero_value       , sizeof(DWORD));
		RegSetValueEx (hkModem, _T("mtu")         , 0, REG_DWORD, (BYTE *)&pp.imtu          , sizeof(DWORD));
		RegSetValueEx (hkModem, _T("hid_subclass"), 0, REG_DWORD, (BYTE *)&zero_value       , sizeof(DWORD));
		RegSetValueEx (hkModem, _T("channel")     , 0, REG_DWORD, (BYTE *)&dwChannel        , sizeof(DWORD));
	}

	// create key that will be used to activate the device
	// YGT Here we have 2 versions : with or w/o the unimodem subkey.
	// Microsoft gives no information about it so we have to try both !!
	StrTCbPrintf ( key, sizeof ( key ), _T("software\\microsoft\\bluetooth\\device\\ports\\%s"), m_Friendlyname);
	HKEY hkPort;
	if (ERROR_SUCCESS == RegCreateKeyEx (HKEY_LOCAL_MACHINE, key, 0, NULL, 0, KEY_WRITE, NULL, &hkPort, &dwDisp))
	{
		dwValue = (DWORD)&pp;
		RegSetValueEx (hkPort, _T("context")     , 0, REG_DWORD, (BYTE *)&dwValue            , sizeof(DWORD)); // Store the address of the context
		dwValue = GetBluetoothFreePort();
		RegSetValueEx (hkPort, _T("index")       , 0, REG_DWORD, (BYTE *)&dwValue            , sizeof(DWORD));
		RegSetValueEx (hkPort, _T("prefix")      , 0, REG_SZ   , (BYTE *)m_PortPrefixe   , (_tcslen(m_PortPrefixe) + 1) * sizeof(_TCHAR));
		RegSetValueEx (hkPort, _T("dll")         , 0, REG_SZ   , (BYTE *)_T("btd.dll")     , sizeof(_T("btd.dll")));

	}

	RegCloseKey ( hkPort);

	m_Handle = ActivateDeviceEx ( key , NULL, 0, NULL );

	if (m_Handle != NULL) 
	{
		Result = true;
	}

	// Finish to fill the modem registry key.
	RegSetValueEx (hkModem, _T("handle"), 0, REG_DWORD, (BYTE *)&m_Handle, sizeof(DWORD));
	dwValue = m_IsActive?1:0;
	RegSetValueEx (hkModem, _T("active"), 0, REG_DWORD, (BYTE *)&dwValue     , sizeof(DWORD));
	RegCloseKey (hkModem);

	return Result;
}
bool CBTDevice::UpdateRegistryModemEntry()
{
	bool Result = false;

	return Result;
}