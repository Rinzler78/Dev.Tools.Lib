#include "BTManager.h"
#include "BTPrinter.h"
#include "BTDevice\BTDevice.h"
#if defined UNDER_CE
#include "CIAC_Tool.h"
#endif
#pragma comment(lib, "bthguid.lib")
CBTManager::CBTManager()
{
}
CBTManager::~CBTManager()
{
}
void * CBTManager::FindBTDevice (const wchar_t * DeviceName, DeviceType_t Device_Type, const int timeout_sec)
{
	#if DEBUG
	CLog Log;
	#endif
	void * Result = NULL;
	CBthInquiryResult * Inquiryresult = NULL;
	wchar_t BtDeviceName[BT_NAME_SIZE] = {0};
	unsigned int count = 0;
	unsigned int ResultSearch = 0;
	#if defined UNDER_CE
	CIAC_Tool IPACmd;
	#endif

	if(DeviceName && timeout_sec != 0)
	{
#if defined UNDER_CE
		/** Active BT if it isn't */
		if(!IPACmd.BluetoothIsActive())
			IPACmd.ActiveBlueTooth();
#endif

		Inquiryresult = GetBthInquiryResult();
		
		if(Inquiryresult && Inquiryresult->GetRequestSuccess())
		{
			// copy the result into the bt_addr array.
			for ( unsigned int i=0; i < Inquiryresult->GetNbDeviceFounded(); i++)
			{
				// get the name
				ResultSearch = BthRemoteNameQuery (	&Inquiryresult->GetBthInquiryResult()[i].ba,	// BT address
											BT_NAME_SIZE,		// buffer size
											&count,				// number of wchar required
											BtDeviceName ); // output buffer

				if((ResultSearch == ERROR_SUCCESS ) && !_tcscmp(BtDeviceName, DeviceName)) 
				{
					switch(Device_Type)
					{
						case COMMON_DEVICE:
							Result = new CBTDevice(BtDeviceName, Inquiryresult->GetBthInquiryResult()[i].ba);
							break;
						case PRINTER_DEVICE:
							Result = new CBTPrinter(BtDeviceName, Inquiryresult->GetBthInquiryResult()[i].ba);
							break;
					}
					
					break;
				}
			}
		}
	}

	/** Free Memory */
	if(Inquiryresult)
	{
		delete Inquiryresult;
		Inquiryresult = NULL;
	}

	return Result;
}
void * CBTManager::FindBTDevice (BT_ADDR DeviceAddr, DeviceType_t Device_Type, const int timeout_sec)
{
	void * Result = NULL;
	CBthInquiryResult * Inquiryresult = NULL;
	wchar_t BtDeviceName[BT_NAME_SIZE] = {0};
	unsigned int count = 0;
	unsigned int ResultSearch = 0;
#if defined UNDER_CE
	CIAC_Tool IPACmd;
#endif

	if(timeout_sec != 0)
	{
#if defined UNDER_CE
		/** Active BT if it isn't */
		if(!IPACmd.BluetoothIsActive())
			IPACmd.ActiveBlueTooth();
#endif

		Inquiryresult = GetBthInquiryResult();
		
		if(Inquiryresult && Inquiryresult->GetRequestSuccess())
		{
			// copy the result into the bt_addr array.
			for ( unsigned int i=0; i < Inquiryresult->GetNbDeviceFounded(); i++)
			{
				if(Inquiryresult->GetBthInquiryResult()[i].ba == DeviceAddr)
				{
					// get the name
					ResultSearch = BthRemoteNameQuery (	&Inquiryresult->GetBthInquiryResult()[i].ba,	// BT address
												BT_NAME_SIZE,		// buffer size
												&count,				// number of wchar required
												BtDeviceName ); // output buffer

					if((ResultSearch == ERROR_SUCCESS )) 
					{
						switch(Device_Type)
						{
							case COMMON_DEVICE:
								Result = new CBTDevice(BtDeviceName, Inquiryresult->GetBthInquiryResult()[i].ba);
								break;
							case PRINTER_DEVICE:
								Result = new CBTPrinter(BtDeviceName, Inquiryresult->GetBthInquiryResult()[i].ba);
								break;
						}
					}
				}
			}
		}
	}

	/** Free Memory */
	if(Inquiryresult)
	{
		delete Inquiryresult;
		Inquiryresult = NULL;
	}

	return Result;
}
CBTDeviceList * CBTManager::GetBTDeviceList(const BYTE MaxNBDevice, const int timeout_sec)
{
	CBTDeviceList * Result = NULL;
	CBthInquiryResult * Inquiryresult = NULL;
	unsigned int NumberOfLetterReturned = 0;
	unsigned int ResultSearch = 0;
	wchar_t BtDeviceName[BT_NAME_SIZE] = {0};
#if defined UNDER_CE
	CIAC_Tool IPACmd;
#endif

	if(MaxNBDevice > 0)
	{
#if defined UNDER_CE
		/** Active BT if it isn't */
		if(!IPACmd.BluetoothIsActive())
			IPACmd.ActiveBlueTooth();
#endif
		
		/** Get List Of BT Device in range */
		Inquiryresult = GetBthInquiryResult();
		
		if(Inquiryresult->GetRequestSuccess())
		{
			if(Inquiryresult->GetNbDeviceFounded() > 0)
			{
				Result = new CBTDeviceList();
				for(BYTE i = 0; i < Inquiryresult->GetNbDeviceFounded(); i++)
				{
					// get the name
					ResultSearch = BthRemoteNameQuery (	&Inquiryresult->GetBthInquiryResult()[i].ba,	// BT address
												BT_NAME_SIZE,		// buffer size
												&NumberOfLetterReturned,				// number of wchar required
												BtDeviceName ); // output buffer

					if((ResultSearch == ERROR_SUCCESS )) 
					{
						const CBTDevice BtDevice(BtDeviceName, Inquiryresult->GetBthInquiryResult()[i].ba);
						Result->push_back(BtDevice);
						memset(&BtDeviceName,0,sizeof(BtDeviceName));
					}
				}
			}
		}
	}

	if(Inquiryresult)
	{
		delete Inquiryresult;
		Inquiryresult = NULL;
	}

	return Result;
}
CBthInquiryResult * CBTManager::GetBthInquiryResult(const BYTE MaxNBDevice, const int timeout_sec)
{
	CBthInquiryResult * Result = NULL;
	BthInquiryResult * InquiryList = NULL;
	int ResultSearch = 0;
	unsigned int NbDeviceFounded = 0;
	bool SearchSuccess = false;
	#if defined UNDER_CE
	CIAC_Tool IPACmd;
#endif

	if(MaxNBDevice > 0)
	{
#if defined UNDER_CE
		/** Active BT if it isn't */
		if(!IPACmd.BluetoothIsActive())
			IPACmd.ActiveBlueTooth();
#endif

		/** Find a list of Bluetooth Device in Range */
		InquiryList = new BthInquiryResult[MaxNBDevice]; 
		memset ( InquiryList, 0, sizeof (BthInquiryResult) * MaxNBDevice);

		ResultSearch = BthPerformInquiry( BT_ADDR_GIAC // General LAP code
								,(unsigned char) (timeout_sec / 1.28) // timeout
								,MaxNBDevice // maximum number of response,
								,sizeof ( BthInquiryResult ) * MaxNBDevice // Size of the receive buffer,
								,&NbDeviceFounded							// Number of discovered devices
								,InquiryList);
		
		/** Result Creation */
		Result = new CBthInquiryResult(ResultSearch == ERROR_SUCCESS, NbDeviceFounded,InquiryList);
	}

	if(InquiryList)
	{
		delete [] InquiryList;
		InquiryList = NULL;
	}

	return Result;
}
HRESULT CBTManager::FindRFCOMMChannel (unsigned char *pStream, int cStream, unsigned char *pChann)
{
	ISdpRecord **pRecordArg;
	int cRecordArg = 0;

	int i, j, k;

	*pChann = 0;

	HRESULT hr = ServiceAndAttributeSearch (pStream, cStream, &pRecordArg, (ULONG *)&cRecordArg);

	if (FAILED(hr))
		return hr;

	for (i = 0; (! *pChann) && (i < cRecordArg); i++) {
	    ISdpRecord *pRecord = pRecordArg[i];    // particular record to examine in this loop
	    NodeData protocolList;     // contains SDP_ATTRIB_PROTOCOL_DESCRIPTOR_LIST data, if available

		if (ERROR_SUCCESS != pRecord->GetAttribute(SDP_ATTRIB_PROTOCOL_DESCRIPTOR_LIST,&protocolList) ||
		    (protocolList.type != SDP_TYPE_CONTAINER))
			continue;
		
		ISdpNodeContainer *pRecordContainer = protocolList.u.container;
		int cProtocols = 0;
		NodeData protocolDescriptor; // information about a specific protocol (i.e. L2CAP, RFCOMM, ...)

		pRecordContainer->GetNodeCount((DWORD *)&cProtocols);
		for (j = 0; (! *pChann) && (j < cProtocols); j++) {
			pRecordContainer->GetNode(j,&protocolDescriptor);

			if (protocolDescriptor.type != SDP_TYPE_CONTAINER)
				continue;

			ISdpNodeContainer *pProtocolContainer = protocolDescriptor.u.container;
			int cProtocolAtoms = 0;
			pProtocolContainer->GetNodeCount((DWORD *)&cProtocolAtoms);

			for (k = 0; (! *pChann) && (k < cProtocolAtoms); k++) {
				NodeData nodeAtom;  // individual data element, such as what protocol this is or RFCOMM channel id.

				pProtocolContainer->GetNode(k,&nodeAtom);

				if (IsRfcommUuid(&nodeAtom))  {
					if (k+1 == cProtocolAtoms) {
						// misformatted response.  Channel ID should follow RFCOMM uuid
						break;
					}

					NodeData channelID;
					pProtocolContainer->GetNode(k+1,&channelID);

					*pChann = (unsigned char)GetChannel(&channelID);
					break; // formatting error
				}
			}
		}
	}

    for (i = 0; i < cRecordArg; i++) 
		pRecordArg[i]->Release();

    CoTaskMemFree(pRecordArg);

	return (*pChann != 0) ? NO_ERROR : E_FAIL;
}
HRESULT CBTManager::ServiceAndAttributeSearch(
    UCHAR *        szResponse,    // in - response returned from SDP ServiceAttribute query
    DWORD          cbResponse,    // in - length of response
    ISdpRecord *** pppSdpRecords, // out - array of pSdpRecords
    ULONG *        pNumRecords    // out - number of elements in pSdpRecords
    )
{
    *pppSdpRecords = NULL;
    *pNumRecords = 0;

    ISdpStream *pIStream = NULL;

    HRESULT hres = CoCreateInstance(__uuidof(SdpStream),NULL,CLSCTX_INPROC_SERVER,
                            __uuidof(ISdpStream),(LPVOID *) &pIStream);

    if (FAILED(hres))
        return hres;  

	ULONG ulError;

    hres = pIStream->Validate (szResponse,cbResponse,&ulError);

    if (SUCCEEDED(hres)) {
        hres = pIStream->VerifySequenceOf(szResponse,cbResponse,
                                          SDP_TYPE_SEQUENCE,NULL,pNumRecords);

        if (SUCCEEDED(hres) && *pNumRecords > 0) {
            *pppSdpRecords = (ISdpRecord **) CoTaskMemAlloc(sizeof(ISdpRecord*) * (*pNumRecords));

            if (pppSdpRecords != NULL) {
                hres = pIStream->RetrieveRecords(szResponse,cbResponse,*pppSdpRecords,pNumRecords);

                if (!SUCCEEDED(hres)) {
                    CoTaskMemFree(*pppSdpRecords);
                    *pppSdpRecords = NULL;
                    *pNumRecords = 0;
                }
            }
            else {
                hres = E_OUTOFMEMORY;
            }
        }
    }

    if (pIStream != NULL) {
        pIStream->Release();
        pIStream = NULL;
    }

	return hres;
}
bool CBTManager::IsRfcommUuid(NodeData *pNode)
{
    if (pNode->type != SDP_TYPE_UUID)
		return false;

    if (pNode->specificType == SDP_ST_UUID16)
	{
        return (pNode->u.uuid16 == RFCOMM_PROTOCOL_UUID16);
    } else if (pNode->specificType == SDP_ST_UUID32)
	{
        return (pNode->u.uuid32 == RFCOMM_PROTOCOL_UUID16);
        // YGT FIXME: Above line should probably check against RFCOMM_PROTOCOL_UUID32.
        // Need to verify this on a 32-bit device, which I don't have. VJA
    } else if (pNode->specificType == SDP_ST_UUID128)
	{
        return (0 == memcmp(&RFCOMM_PROTOCOL_UUID,&pNode->u.uuid128,sizeof(GUID)));
    }
    return false;
}

int CBTManager::GetChannel (NodeData *pChannelNode)
{
    if (pChannelNode->specificType == SDP_ST_UINT8)
        return pChannelNode->u.uint8;
    else if (pChannelNode->specificType == SDP_ST_INT8)
        return pChannelNode->u.int8;
    else if (pChannelNode->specificType == SDP_ST_UINT16)
        return pChannelNode->u.uint16;
    else if (pChannelNode->specificType == SDP_ST_INT16)
        return pChannelNode->u.int16;
    else if (pChannelNode->specificType == SDP_ST_UINT32)
        return pChannelNode->u.uint32;
    else if (pChannelNode->specificType == SDP_ST_INT32)
        return pChannelNode->u.int32;

    return 0;
}