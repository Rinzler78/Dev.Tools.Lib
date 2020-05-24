#ifndef CBASECONNECTEDCLIENT_H
#define CBASECONNECTEDCLIENT_H
#include "Common\MyTypes.h"
#include "BaseClientCommunication\BaseClientCommunication.h"
#include "BaseConnectedClient\BaseConnectedClient_Defs.h"

class CBaseConnectedClient_Delegate;

class CBaseConnectedClient : public CBaseClientCommunication
{
public:
	CBaseConnectedClient(BaseClientConnectionType_t ConnectionObjectID, CBaseConnectedClient_Delegate * pDelegate, CLog * pLog = NULL);
	~CBaseConnectedClient();

	/** Connection to target */
	bool Connect();
	bool ConnectAsync();

	inline bool Connected()const {return m_ConnectionStatus == BaseConnectedClientStatus_Connected;};
	inline BaseConnectedClientStatus_t ConnectionStatus()const {return m_ConnectionStatus;};

	/** Disconnection from target */
	bool DisConnect();
	bool DisConnectAsync();

	/** Reconnection */
	bool Reconnect();
	bool ReconnectAsync();

	inline CBaseConnectedClient_Delegate * Delegate()const {return (CBaseConnectedClient_Delegate *)CBaseClientCommunication::Delegate();}

protected:
	/** Connection */
	virtual bool VWillConnect() = NULL;
	virtual bool VConnect() = NULL;
	virtual bool VDidConnect() = NULL;

	/** DisConnection */
	virtual bool VWillDisConnect() = NULL;
	virtual bool VDisConnect() = NULL;
	virtual bool VDidDisConnect() = NULL;

	/** Send */
	virtual TU32_t VSend(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier = NULL) = NULL; 
	/** Receive */
	virtual TU32_t VReceive(TU08_t * pOutBuffer, TU32_t outSize) = NULL;
private:
	void Event_FrameSendResult(TU08_t * pBuffer, TU32_t BufferSize, TU32_t NBBytesSent, TU32_t FrameIDentifier);
	void Event_FrameReceived(TU08_t * pBuffer, TU32_t Size, TU32_t SizeWaited);
	
	/** Send */
	TU32_t VSendData(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier = NULL); 
	/** Receive */
	TU32_t VReceiveData(TU08_t * pOutBuffer, TU32_t outSize);
	/** Action Async Execution */
	void ExecuteAction(TU32_t ActionID);

	/** Connection Status */
	void SetConnectionStatus(BaseConnectedClientStatus_t ConnectionStatus);
	BaseConnectedClientStatus_t m_ConnectionStatus;

	/** Connexion */
	CMutex * m_pConnexionMutex;

	/** DisConnexion */
	CMutex * m_pDisConnexionMutex;
};
#endif