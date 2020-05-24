#ifndef CBASEUDPCLIENT_H
#define CBASEUDPCLIENT_H
#include "Common\MyTypes.h"
#include "BaseIp\BaseIp.h"
#include "BaseClientCommunication\BaseClientCommunication.h"

class CBaseUDPClient : public CBaseIP, public CBaseClientCommunication
{
public:
	CBaseUDPClient(CBaseClientCommunication_Delegate * pDelegate, TS08_t ConnectionString, CLog * pLog = NULL);
	~CBaseUDPClient();
protected:
	bool CreateSocket();
private:
	/** Send */
	TU32_t VSendData(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier = NULL); 
	/** Receive */
	TU32_t VReceiveData(TU08_t * pOutBuffer, TU32_t outSize);
};
#endif