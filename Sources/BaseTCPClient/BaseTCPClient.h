#ifndef CBASETCPCLIENT_H
#define CBASETCPCLIENT_H
#include "Common\MyTypes.h"
#include "BaseIp\BaseIp.h"
#include "BaseConnectedClient\BaseConnectedClient.h"

class CBaseTCPClient : public CBaseIP, public CBaseConnectedClient
{
public:
	CBaseTCPClient(CBaseConnectedClient_Delegate * pDelegate, TS08_t ConnectionString, CLog * pLog = NULL);
	~CBaseTCPClient();
protected:
	bool CreateSocket();
private:
	/** Connection */
	bool VWillConnect();
	bool VConnect();
	bool VDidConnect();

	/** DisConnection */
	bool VWillDisConnect();
	bool VDisConnect();
	bool VDidDisConnect();

	/** Send */
	TU32_t VSend(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier = NULL); 
	/** Receive */
	TU32_t VReceive(TU08_t * pOutBuffer, TU32_t outSize);
};
#endif