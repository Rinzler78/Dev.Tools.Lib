#include "BaseClient.h"
#include "Thread\Thread.h"
#include "Mutex\Mutex.h"
#include "Log\Log.h"
#include "Clock\Clock.h"
#define TEST_INCREMENTIAL_BUFFER

#define SINGLE_THREAD
CBaseClient::CBaseClient(ConnectionType_t Type
						, CBaseClientDelegate * pDelegate
						, CLog * pLog)
:m_ConnectionType(Type)
, m_ConnectionState(ClientConnectionState_DisConnected)
, m_pDelegate(pDelegate)
, m_pLog(pLog)
, m_NbBytesWaited(0)
, m_Connected(false)
, m_pActionThread(NULL)
, m_AsyncMutex(NULL)
, m_ConnectionMutex(NULL)
, m_WaitForExit(false)
, m_KillEvent(NULL)
#if defined (FIFO_IMPLEMENTATION)
, m_TopEvent(INVALID_HANDLE_VALUE)
, m_ActionListMutex(NULL)
#endif
, m_TotalNumberOfBytesSent(0)
, m_TotalNumberOfBytesReceived(0)
, m_ReceptionMutex(NULL)
, m_SendMutex(NULL)
{
	/** Init Times */
	MACRO_MEMSET(&m_TimeToConnect, sizeof(m_TimeToConnect))
	MACRO_MEMSET(&m_TimeToDisConnect, sizeof(m_TimeToDisConnect))
	MACRO_MEMSET(&m_TimeToReceive, sizeof(m_TimeToReceive))
	MACRO_MEMSET(&m_TimeToSend, sizeof(m_TimeToSend))

	/** Create Async Mutex */
	m_AsyncMutex = new CMutex(L"Base Client Async Mutex");
	m_ConnectionMutex = new CMutex(L"Connection Async Mutex");
	m_ReceptionMutex = new CMutex(L"Reception Mutex");
	m_SendMutex = new CMutex(L"Send Mutex");
	
	/** Kill */
	m_KillEvent = CreateEvent(NULL,false,false,NULL);

	/** Init Handle Tab */
#if defined (FIFO_IMPLEMENTATION)
	m_ActionListMutex = new CMutex(L"Action List Mutex");
	m_TopEvent = CreateEvent(NULL,false,false,NULL);
#else
	MACRO_MEMSET(m_TabEvents, sizeof(m_TabEvents));
	for(TU32_t i = 0 ; i < MAX_BaseClient_Event ; ++i)
		m_TabEvents[i] = CreateEvent(NULL,false,false,NULL);
#endif

	/** init Action thread */
	m_pActionThread = new CThread(	"Base Client Action Thread"
													, ActionThreadFunction
													, m_pLog);
	m_pActionThread->Start(this);
}
CBaseClient::~CBaseClient()
{
	/** Close Action thread */
	SetEvent(m_KillEvent);
	AddAsyncAction(BaseClient_Event_Exit);
	CloseHandle(m_KillEvent);
	
	SetEvent(m_TopEvent);
	MACRO_DELET(m_pActionThread)
	CloseHandle(m_TopEvent);

	/** Realease Mutex */
	MACRO_DELET(m_AsyncMutex)
	MACRO_DELET(m_ConnectionMutex)
	MACRO_DELET(m_SendMutex)
	MACRO_DELET(m_ReceptionMutex)

	while(m_ActionList.Count())
		GetFirstAsyncAction();

	MACRO_DELET(m_ActionListMutex)
}
/** Connection to target */
bool CBaseClient::Connect()
{
	bool Result = false;
	
	m_ConnectionMutex->Wait();
	if(!Connected())
	{
		m_TimeToConnect = CClock::GetLocalTime();
		SetConnectionState(ClientConnectionState_TryToConnect, &m_TimeToConnect);

		if(!VWillConnect())
			if(m_pLog)m_pLog->Log(NOCOMMENTS, "Will Connect Failure");

		if(Result = VConnect())
		{
			m_Connected = Result;
			if(!VDidConnect())
				if(m_pLog)m_pLog->Log(NOCOMMENTS, "Did Connect Failure");
			
			m_TimeToConnect = CClock::GetLocalTime().GetElapsedTime(m_TimeToConnect);
			SetConnectionState(ClientConnectionState_Connected, &m_TimeToConnect);
		}
		else
		{
			m_TimeToConnect = CClock::GetLocalTime().GetElapsedTime(m_TimeToConnect);
			SetConnectionState(ClientConnectionState_ConnectionFailure, &m_TimeToConnect);
			SetConnectionState(ClientConnectionState_DisConnected, &m_TimeToConnect);
		}
	}

	m_ConnectionMutex->Release();

	return Result;
}
bool CBaseClient::ConnectAsync()
{
	bool Result = false;
	
	if(m_AsyncMutex)m_AsyncMutex->Wait();

	if(!Connected())
	{
		if(LogObject())
			LogObject()->Log(NOCOMMENTS, "Connect Async");
#if defined (FIFO_IMPLEMENTATION)
		Result = AddAsyncAction(BaseClient_Event_Connect);
#else
		Result = (bool)SetEvent(m_TabEvents[BaseClient_Event_Connect]);
#endif
	}

	if(m_AsyncMutex)m_AsyncMutex->Release();

	return Result;
}

/** Disconnection from target */
bool CBaseClient::DisConnect()
{
	bool Result = false;
	
	m_ConnectionMutex->Wait();
	if(Connected())
	{
		m_TimeToDisConnect = CClock::GetLocalTime();
		SetConnectionState(ClientConnectionState_TryToDisConnect, &m_TimeToDisConnect);

		if(!VWillDisConnect())
			if(m_pLog)m_pLog->Log(NOCOMMENTS, "Will DisConnect Failure");

		/** Send All Frame in list before disconnect */
		if(m_AsyncMutex)m_AsyncMutex->Wait();
		while(m_SendFrameList.Count())
		{
			CFrame * pFrame = m_SendFrameList[0];
			if(pFrame)
			{
				Send(pFrame->FrameBuffer(),pFrame->FrameBufferSize(),pFrame->FrameIdentifier());
			}

			m_SendFrameList.DeleteFirst();
			
			MACRO_DELET(pFrame);
		}

		if(m_AsyncMutex)m_AsyncMutex->Release();

		if(Result = VDisConnect())
		{
			m_Connected = !Result;
			if(!VDidDisConnect())
				if(m_pLog)m_pLog->Log(NOCOMMENTS, "Did DisConnect Failure");
			
			m_TimeToDisConnect = CClock::GetLocalTime().GetElapsedTime(m_TimeToDisConnect);
			SetConnectionState(ClientConnectionState_DisConnected, &m_TimeToDisConnect);
		}
		else
		{
			m_TimeToDisConnect = CClock::GetLocalTime().GetElapsedTime(m_TimeToDisConnect);
			SetConnectionState(ClientConnectionState_DisConnectionFailure, &m_TimeToDisConnect);
			SetConnectionState(ClientConnectionState_Connected, &m_TimeToDisConnect);
		}
	}
	m_ConnectionMutex->Release();

	return Result;
}
bool CBaseClient::DisConnectAsync()
{
	bool Result = false;
	
	if(m_AsyncMutex)m_AsyncMutex->Wait();

	if(Connected())
	{
		if(LogObject())
			LogObject()->Log(NOCOMMENTS, "DisConnect Async");
		
#if defined (FIFO_IMPLEMENTATION)
		Result = AddAsyncAction(BaseClient_Event_DisConnect);
#else
		Result = (bool)SetEvent(m_TabEvents[BaseClient_Event_DisConnect]);
#endif
		m_WaitForExit = true;
	}
	
	if(m_AsyncMutex)m_AsyncMutex->Release();

	return Result;
}

/** Reconnection */
bool CBaseClient::Reconnect()
{
	bool Result = false;
	if(m_AsyncMutex)m_AsyncMutex->Wait();
	
	if(Connected())
	{
		if(DisConnect())
			Connect();
	}

	if(m_AsyncMutex)m_AsyncMutex->Release();

	return Result;
}
bool CBaseClient::ReconnectAsync()
{
	bool Result = false;
	
	if(m_AsyncMutex)m_AsyncMutex->Wait();

	if(Connected())
	{
		if(LogObject())
			LogObject()->Log(NOCOMMENTS, "ReConnect Async");
		
#if defined (FIFO_IMPLEMENTATION)
		Result = AddAsyncAction(BaseClient_Event_ReConnect);
#else
		Result = (bool)SetEvent(m_TabEvents[BaseClient_Event_ReConnect]);
#endif
	}
	
	if(m_AsyncMutex)m_AsyncMutex->Release();

	return Result;
}
/** Frame Reception */
TU32_t CBaseClient::Receive(TU08_t * pOutBuffer, TU32_t NbBytesWaited)
{
	TU32_t Result = 0;
	TU32_t NbByteReceived = 0;
	TU32_t NbByteToReceive = 0;
	TU32_t RetryReceiveCount = 3;
	
	if(m_ReceptionMutex)m_ReceptionMutex->Wait();
	if(Connected() && pOutBuffer && (NbBytesWaited > 0))
	{
		m_TimeToReceive = CClock::GetLocalTime();
		SetConnectionState(ClientConnectionState_WaitingForFrame, &m_TimeToReceive);
		
		do
		{
			NbByteToReceive = NbBytesWaited - Result;
			NbByteReceived = VReceive(pOutBuffer + Result, NbByteToReceive);
			Result += NbByteReceived;
			
			if(m_pLog)
				m_pLog->Log(NOCOMMENTS
							, "Nb Byte To Receive : %u\nNb Byte Received : %u\nTotal Nb Byte Received : %u"
							, NbByteToReceive
							, NbByteReceived
							, Result);

			m_TotalNumberOfBytesReceived += NbByteReceived;
			if(m_pLog)
				m_pLog->Log(NOCOMMENTS, "Total Number Of Bytes Received on channel : %u", m_TotalNumberOfBytesReceived);

			if(Result && (Result < NbBytesWaited) && !NbByteReceived)
			{
				Sleep(100);
				--RetryReceiveCount;
				if(m_pLog)
				m_pLog->Log(NOCOMMENTS, "Retry to receive");
			}
		}
		while((Result && (Result < NbBytesWaited) && (NbByteReceived || RetryReceiveCount)));

		if(Result == NbBytesWaited)
		{
			m_TimeToReceive = CClock::GetLocalTime().GetElapsedTime(m_TimeToReceive);
			SetConnectionState(ClientConnectionState_FrameReceived, &m_TimeToReceive);
		}
	}
	if(m_ReceptionMutex)m_ReceptionMutex->Release();

	return Result;
}
bool CBaseClient::ReceiveAsync(TU32_t NbBytesWaited)
{
	bool Result = false;
	
	if(m_AsyncMutex)m_AsyncMutex->Wait();

	if(Connected() && !m_WaitForExit && (NbBytesWaited  > 0))
	{
		m_NbBytesWaited = NbBytesWaited;
		Result = AddAsyncAction(BaseClient_Event_Receive);
	}

	if(m_AsyncMutex)m_AsyncMutex->Release();

	return Result;
}

/** Frame Send */
TU32_t CBaseClient::Send(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier)
{
	TU32_t Result = 0;
	TU32_t NbByteSent = 0;
	TU32_t NbByteToSend = 0;
	bool SendSuccess = false;
	
	if(m_SendMutex)m_SendMutex->Wait();
	if(Connected() && pSendBuffer && Size)
	{
		m_TimeToSend = CClock::GetLocalTime();
		SetConnectionState(ClientConnectionState_TryToSendFrame, &m_TimeToSend);
		
		/*Result = VSend(pSendBuffer + Result, Size, FrameIdentifier);
		
		if(m_pLog)
				m_pLog->Log(NOCOMMENTS
							, "\nNb Byte To Send : %u\nNb Byte sent : %u"
							, Size
							, Result);*/

		do
		{
			NbByteToSend = Size - Result;
			NbByteSent = VSend(pSendBuffer + Result, NbByteToSend, FrameIdentifier);
			Result += NbByteSent;
			if(m_pLog)
				m_pLog->Log(NOCOMMENTS
							, "Nb Byte To Send : %u\nNb Byte sent : %u\nTotal Nb Byte Sent : %u"
							, NbByteToSend
							, NbByteSent
							, Result);

			m_TotalNumberOfBytesSent += NbByteSent;
			if(m_pLog)
				m_pLog->Log(NOCOMMENTS, "Total Number Of Bytes Sent on channel : %u", m_TotalNumberOfBytesSent);
		}
		while((Result < Size) && (NbByteSent));
		
		SendSuccess = ((Result > 0) || (GetLastError() == 0));
		if(SendSuccess)
		{
			m_TimeToSend = CClock::GetLocalTime().GetElapsedTime(m_TimeToSend);
			SetConnectionState(ClientConnectionState_FrameSent, &m_TimeToSend);
		}

		if(m_pDelegate)
			m_pDelegate->CBaseClient_Event_FrameSendResult(SendSuccess, pSendBuffer, Size, FrameIdentifier);
	}
	if(m_SendMutex)m_SendMutex->Release();

	return Result;
}
bool CBaseClient::SendAsync(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier)
{
	bool Result = false;

	if(m_AsyncMutex)m_AsyncMutex->Wait();
	
	if(Connected() && !m_WaitForExit && pSendBuffer && Size)
	{
		/** Add Frame To List */
		CFrame * pFrameToSend = new CFrame(pSendBuffer, Size, FrameIdentifier);
		m_SendFrameList.Add(pFrameToSend);
#if defined (FIFO_IMPLEMENTATION)
		Result = AddAsyncAction(BaseClient_Event_Send);
#else
		Result = (bool)SetEvent(m_TabEvents[BaseClient_Event_Send]);
#endif
	}

	if(m_AsyncMutex)m_AsyncMutex->Release();

	return Result;
}
void CBaseClient::SetConnectionState(ClientConnectionState_t ConnexionState, CClock * pElapsedTime)
{
	m_ConnectionState = ConnexionState;
	
	if(LogObject())
		LogObject()->Log(NOCOMMENTS	, "Connection State Change -> %s (%u:%u:%u)"
									, ClientConnectionState_STR_TAB[ConnexionState]
									, pElapsedTime->Hour()
									, pElapsedTime->Minutes()
									, pElapsedTime->MilliSecondes());

	if(m_pDelegate)
		m_pDelegate->CBaseClient_Event_ConnectionState(m_ConnectionState, pElapsedTime);
}
bool CBaseClient::AddAsyncAction(BaseClient_Event_t Action, TU32_t NbSecondToSleepAfter, bool HighPriority)
{
	bool Result = false;
	BaseClient_Event_t * pActionToPost = NULL;

	if(Action < MAX_BaseClient_Event)
	{
		pActionToPost = new BaseClient_Event_t;
		*pActionToPost = Action;
		
		m_ActionListMutex->Wait();
		
		if(!HighPriority)
			m_ActionList.Add(pActionToPost);
		else
			m_ActionList.InsertAt(0, pActionToPost);

		m_ActionListMutex->Release();

		Result = (SetEvent(m_TopEvent) == TRUE);

		if(Action == BaseClient_Event_Exit)
		{
			WaitForSingleObject(m_KillEvent, INFINITE);
		}
	}

	return Result;
}
BaseClient_Event_t CBaseClient::GetFirstAsyncAction()
{
	BaseClient_Event_t Result = MAX_BaseClient_Event;
	
	m_ActionListMutex->Wait();

	if(m_ActionList.Count())
	{
		BaseClient_Event_t * pAction = m_ActionList[0];
		if(pAction)
		{
			Result = *pAction;
			MACRO_DELET(pAction)
		}
		m_ActionList.DeleteFirst();
	}

	m_ActionListMutex->Release();

	return Result;
}
void ActionThreadFunction(void * pParams)
{
	DWORD Result = 0;
	if(pParams)
	{
		CThread * pThread = (CThread *)pParams;
		CBaseClient * pBaseClient = (CBaseClient *)pThread->ThreadParam();
		
		while((pThread->State() == CThread_State_Running))
		{
#if !defined (FIFO_IMPLEMENTATION)
			Result = WaitForMultipleObjects(MAX_BaseClient_Event, pBaseClient->m_TabEvents, false, INFINITE);
#else
			Result = WaitForSingleObject(pBaseClient->m_TopEvent, INFINITE);
			Result = pBaseClient->GetFirstAsyncAction();
#endif
			
			if(Result == BaseClient_Event_Exit)
			{
				pBaseClient->DisConnect();
				break;
			}

			if(Result < MAX_BaseClient_Event)
			{
				switch(Result)
				{
					case BaseClient_Event_Connect:
						{
							/** Try To Connect */
							pBaseClient->Connect();
						}
						break;
					case BaseClient_Event_DisConnect:
						{
							if(pBaseClient->DisConnect())
								pBaseClient->m_WaitForExit = false;
						}
						break;
					case BaseClient_Event_ReConnect:
						pBaseClient->Reconnect();
						break;
					case BaseClient_Event_Send:
						{
							unsigned int NbBytesSent = 0;

							if(pBaseClient->m_SendFrameList.Count())
							{
								CFrame * pFrame = (CFrame *)pBaseClient->m_SendFrameList[0];
								/** Remove Frame from List */
								pBaseClient->m_SendFrameList.DeleteFirst();

								if(pFrame)
								{
									NbBytesSent = pBaseClient->Send(pFrame->FrameBuffer(), pFrame->FrameBufferSize(), pFrame->FrameIdentifier());

									if((NbBytesSent > 0) && (NbBytesSent < pFrame->FrameBufferSize()))
									{
										pBaseClient->SendAsync(	pFrame->FrameBuffer() + NbBytesSent
																, (pFrame->FrameBufferSize() - NbBytesSent) 
																, pFrame->FrameIdentifier());
									}
									MACRO_DELET(pFrame)
								}
								
#if defined (FIFO_IMPLEMENTATION)
								if(pBaseClient->m_SendFrameList.Count() && !pBaseClient->m_WaitForExit)
									pBaseClient->AddAsyncAction(BaseClient_Event_Send);
#else
								if(pBaseClient->m_SendFrameList.Count() && !pBaseClient->m_WaitForExit)
									SetEvent(pBaseClient->m_TabEvents[BaseClient_Event_Send]);
#endif
							}
						}
						break;
					case BaseClient_Event_Receive:
						{
							TU32_t SizeReaded = 0;
							TU08_t * pReceiveBuffer = NULL;

							/** Try To Receive */
							pReceiveBuffer = new TU08_t[pBaseClient->m_NbBytesWaited];
							MACRO_MEMSET(pReceiveBuffer, pBaseClient->m_NbBytesWaited);
							
							SizeReaded = pBaseClient->Receive(pReceiveBuffer, pBaseClient->m_NbBytesWaited);
							if(SizeReaded)
							{
								/** Deliver Frame */
								if(pBaseClient->m_pLog)
									pBaseClient->m_pLog->Log(NOCOMMENTS, (SizeReaded > 1 ? "%u/%u Bytes received": "%u/%u Byte received"), SizeReaded, pBaseClient->m_NbBytesWaited);
								
								if(pBaseClient->m_pDelegate)
									pBaseClient->m_pDelegate->CBaseClient_Event_FrameReceived(pReceiveBuffer, SizeReaded);
							}
							else if(!pBaseClient->m_WaitForExit)
							{
								//pBaseClient->AddAsyncAction(BaseClient_Event_Sleep);
								pBaseClient->AddAsyncAction(BaseClient_Event_Receive);
							}

							/** Delete Frame */
							MACRO_DELET_TAB(pReceiveBuffer)
						}
						break;
						case BaseClient_Event_Sleep:
							Sleep(500);
							break;
					default:
						break;
				}
			}

			if(pBaseClient->m_ActionList.Count() > 0)
				SetEvent(pBaseClient->m_TopEvent);
		}

		SetEvent(pBaseClient->m_KillEvent);
	}
}