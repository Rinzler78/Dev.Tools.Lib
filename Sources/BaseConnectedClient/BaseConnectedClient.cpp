#include "BaseConnectedClient\BaseConnectedClient.h"
#include "Mutex\Mutex.h"
#include "Log\Log.h"
#include "BaseConnectedClient\BaseConnectedClient_Delegate.h"
#include "BaseConnectedClient\BaseConnectedClient_Defs.h"

CBaseConnectedClient::CBaseConnectedClient(BaseClientConnectionType_t ConnectionObjectID, CBaseConnectedClient_Delegate * pDelegate, CLog * pLog)
:CBaseClientCommunication(ConnectionObjectID, pDelegate, pLog)
, m_pConnexionMutex(new CMutex(L"Connexion Mutex"))
, m_pDisConnexionMutex(new CMutex(L"DisConnexion Mutex"))
, m_ConnectionStatus(BaseConnectedClientStatus_Disconnected)
{
}
CBaseConnectedClient::~CBaseConnectedClient()
{
	AddAsyncAction(BaseClientCommunication_Event_Exit, 0, true);
	MACRO_DELET(m_pConnexionMutex)
	MACRO_DELET(m_pDisConnexionMutex)
}

/** Connection to target */
bool CBaseConnectedClient::Connect()
{
	bool Result = false;

	if(LogObject())
		LogObject()->Log(NOCOMMENTS, "Try Connect");
	
	if(m_pConnexionMutex)
			m_pConnexionMutex->Wait();

	if(!Connected())
	{
		SetConnectionStatus(BaseConnectedClientStatus_TryConnect);

		Result = VConnect();
		
		if(LogObject())
			LogObject()->Log(NOCOMMENTS, "Connection %s", (Result ? "Success":"Failure"));

		if(Result)
			SetConnectionStatus(BaseConnectedClientStatus_Connected);
		else
		{
			SetConnectionStatus(BaseConnectedClientStatus_ConnectionFailure);
			SetConnectionStatus(BaseConnectedClientStatus_Disconnected);
		}
	}
	else
		if(LogObject())
			LogObject()->Log(NOCOMMENTS, "Allready Connected");	

	if(m_pConnexionMutex)
		m_pConnexionMutex->Release();

	return Result;
}
bool CBaseConnectedClient::ConnectAsync()
{
	return AddAsyncAction(BaseConnectedClient_Event_Connection);
}

/** Disconnection from target */
bool CBaseConnectedClient::DisConnect()
{
	bool Result = false;

	if(LogObject())
		LogObject()->Log(NOCOMMENTS, "Try DisConnect");
	
	if(m_pDisConnexionMutex)
		m_pDisConnexionMutex->Wait();

	if(Connected())
	{
		SetConnectionStatus(BaseConnectedClientStatus_TryDisconnect);

		Result = VDisConnect();
		
		if(LogObject())
			LogObject()->Log(NOCOMMENTS, "DisConnection %s", (Result ? "Success":"Failure"));

		if(Result)
			SetConnectionStatus(BaseConnectedClientStatus_Disconnected);
		else
		{
			SetConnectionStatus(BaseConnectedClientStatus_DisconnectionFailure);
			SetConnectionStatus(BaseConnectedClientStatus_Connected);
		}
	}
	else
		if(LogObject())
			LogObject()->Log(NOCOMMENTS, "Allready DisConnected");	

	if(m_pDisConnexionMutex)
			m_pDisConnexionMutex->Release();

	return Result;
}
bool CBaseConnectedClient::DisConnectAsync()
{
	return AddAsyncAction(BaseConnectedClient_Event_Disconnection);
}

/** Reconnection */
bool CBaseConnectedClient::Reconnect()
{
	if(DisConnect())
		return Connect();

	return false;
}
bool CBaseConnectedClient::ReconnectAsync()
{
	return AddAsyncAction(BaseConnectedClient_Event_Reconnection);
}
TU32_t CBaseConnectedClient::VSendData(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier)
{
	TU32_t Result = 0;

	if(m_pDisConnexionMutex)
		m_pDisConnexionMutex->Wait();

	if(Connected())
		Result = VSend(pSendBuffer, Size, FrameIdentifier);

	Sleep(2000);

	if(m_pDisConnexionMutex)
			m_pDisConnexionMutex->Release();

	return Result;
}
/** Receive */
TU32_t CBaseConnectedClient::VReceiveData(TU08_t * pOutBuffer, TU32_t outSize)
{
	TU32_t Result = 0;
	
	if(m_pDisConnexionMutex)
		m_pDisConnexionMutex->Wait();

	if(Connected())
		Result = VReceive(pOutBuffer, outSize);

	if(m_pDisConnexionMutex)
		m_pDisConnexionMutex->Release();

	return Result;
}
void CBaseConnectedClient::ExecuteAction(TU32_t ActionID)
{
	if((ActionID < MAX_BaseConnectedClient_Event) && (ActionID > BaseConnectedClient_Event_Init))
	{
		BaseConnectedClient_Event_t Action = (BaseConnectedClient_Event_t)ActionID;

		switch(ActionID)
		{
			case BaseConnectedClient_Event_Connection:
				Connect();
				break;
			case BaseConnectedClient_Event_Disconnection:
				DisConnect();
				break;
			case BaseConnectedClient_Event_Reconnection:
				Reconnect();
				break;
			default:
				break;
		}
	}
}
void CBaseConnectedClient::SetConnectionStatus(BaseConnectedClientStatus_t ConnectionStatus)
{
	if((ConnectionStatus < MAX_BaseConnectedClientStatus) && (Delegate()))
	{
		m_ConnectionStatus = ConnectionStatus;
		Delegate()->Event_ConnectionStateChanged(m_ConnectionStatus);
	}
}