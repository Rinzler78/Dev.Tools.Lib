#ifndef CBTMANAGER_H
#define CBTMANAGER_H
#define BT_NAME_SIZE		256
#define BT_PORT_NAME_SIZE	6
#define DEFAULT_DEVICE_SEARCH_TIMEOUT 10 /** In s */
#define DEFAULT_MAX_NUMBER_OF_DEVICE_LIST 100

#include "BthInquiryResult\BthInquiryResult.h"
#include "BTDevice\BTDevice.h"
#if DEBUG
#include "Log\Log.h"
#endif

typedef enum DeviceType
{
	COMMON_DEVICE,
	PRINTER_DEVICE,
	MAX_DEVICE_TYPE
}DeviceType_t;

class CBTManager
{
public:
	CBTManager();
	~CBTManager();
	static void * FindBTDevice (const wchar_t * DeviceName, DeviceType_t Device_Type, const int timeout_sec = DEFAULT_DEVICE_SEARCH_TIMEOUT);
	static void * FindBTDevice (BT_ADDR DeviceAddr, DeviceType_t Device_Type, const int timeout_sec = DEFAULT_DEVICE_SEARCH_TIMEOUT);
	static CBTDeviceList * GetBTDeviceList(const BYTE MaxNBDevice = DEFAULT_MAX_NUMBER_OF_DEVICE_LIST, const int timeout_sec = DEFAULT_DEVICE_SEARCH_TIMEOUT);
	static CBthInquiryResult * GetBthInquiryResult(const BYTE MaxNBDevice = DEFAULT_MAX_NUMBER_OF_DEVICE_LIST, const int timeout_sec = DEFAULT_DEVICE_SEARCH_TIMEOUT);
	static HRESULT FindRFCOMMChannel (unsigned char *pStream, int cStream, unsigned char *pChann);
	static HRESULT ServiceAndAttributeSearch(UCHAR *szResponse, DWORD cbResponse, ISdpRecord *** pppSdpRecords, ULONG * pNumRecords);
	static int GetChannel (NodeData *pChannelNode);
	static bool IsRfcommUuid(NodeData *pNode);
private:
};

#endif