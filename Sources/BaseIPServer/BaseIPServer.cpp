#include "BaseIPServer.h"
#include "Log\Log.h"
#include "Thread\Thread.h"
#include "Mutex\Mutex.h"

CBaseIPServer::CBaseIPServer(TS08_t ConnectionString, TU16_t NbMaxClientAtTheSameTime, CLog * pLog)
:CBaseIP(BaseIPType_IP_Server, ConnectionString,pLog)
, m_pServerThread(NULL)
, m_pLog(pLog)
, m_NbMaxClientAtTheSameTime(NbMaxClientAtTheSameTime)
, m_SyncMutex(NULL)
, m_ServerState(CBaseIPServer_ServerState_Stoped)
{
	/** Create Events */
	MACRO_MEMSET(m_TabEvents, sizeof(m_TabEvents))

	/** Creat Async Mutex */
	m_SyncMutex = new CMutex(L"Server Async mutex");

	for(TU08_t i = 0 ; i < MAX_CBaseIPServer_Events; ++i)
		m_TabEvents[i] = CreateEvent(NULL,false,false,NULL);

	/** Create Server thread */
	m_pServerThread = new CThread("", ServerThreadFunction, m_pLog);
	
	/** Start thread */
	m_pServerThread->Start(this);
}

CBaseIPServer::~CBaseIPServer()
{
	/** Clean Server thread */
	SetEvent(m_TabEvents[CBaseIPServer_Events_Exit]);
	CloseSocket();
	MACRO_DELET(m_pServerThread)

	/** Clean Events */
	for(TU08_t i = 0 ; i < MAX_CBaseIPServer_Events; ++i)
		CloseHandle(m_TabEvents[i]);

	MACRO_DELET(m_SyncMutex)
}
bool CBaseIPServer::CreateSocket()
{
	return false;
}
bool CBaseIPServer::Start()
{
	bool Result = false;
	
	m_SyncMutex->Wait();

	if(m_ServerState == CBaseIPServer_ServerState_Stoped)
	{
		if(Log())
			Log()->Log(NOCOMMENTS, "Start");

		CreateSocket();

		if(bind (Socket(), (const SOCKADDR *)TargetAddrStructure(), sizeof(*(TargetAddrStructure()))) == SOCKET_ERROR)
		{
			if(Log())
				Log()->Log(NOCOMMENTS, "Bind failure (%u)", WSAGetLastError());

			CloseSocket();
		}
		else
		{
			/** Set Socket options */
			TI16_t optval = 0;
			TI16_t optValLen = 0;

			if(getsockopt(Socket(), SOL_SOCKET, SO_ACCEPTCONN | SO_KEEPALIVE | SO_REUSEADDR, (TC08_t *)&optval, (int *)&optValLen) != 0)
				if(Log())
					Log()->Log(NOCOMMENTS, "Get SockOpt failure (%u)", WSAGetLastError());

			if(setsockopt(Socket(), SOL_SOCKET, SO_ACCEPTCONN | SO_KEEPALIVE | SO_REUSEADDR, (TC08_t *)&optval, optValLen) != 0)
				if(Log())
					Log()->Log(NOCOMMENTS, "Set SockOpt failure (%u)", WSAGetLastError());

			/** Launch Listening */
			if(listen(Socket(), m_NbMaxClientAtTheSameTime) == SOCKET_ERROR)
			{
				if(Log())
					Log()->Log(NOCOMMENTS, "Listen failure (%u)", WSAGetLastError());
			}
			else
			{
				Result=true;
				SetEvent(m_TabEvents[CBaseIPServer_Events_WaitForClientConnection]);
			}
		}
	}

	m_SyncMutex->Release();

	return Result;
}
bool CBaseIPServer::Stop()
{
	bool Result = false;
	
	m_SyncMutex->Wait();

	if(m_ServerState != CBaseIPServer_ServerState_Stoped)
	{
		CloseSocket();
	}

	m_SyncMutex->Release();

	return Result;
}
void ServerThreadFunction(void * pParams)
{
	DWORD Result = 0;
	if(pParams)
	{
		CThread * pThread = (CThread *)pParams;
		CBaseIPServer * pBaseIPServer = (CBaseIPServer *)pThread->ThreadParam();
		
		while((pThread->State() == CThread_State_Running))
		{
			Result = WaitForMultipleObjects(MAX_CBaseIPServer_Events, pBaseIPServer->m_TabEvents, false, INFINITE);

			if(Result < MAX_CBaseIPServer_Events)
			{
				if(Result == CBaseIPServer_Events_Exit)
					break;

				switch(Result)
				{
					case CBaseIPServer_Events_WaitForClientConnection:
						{
							if(pBaseIPServer->Log())
								pBaseIPServer->Log()->Log(NOCOMMENTS, "Wait for Connection");

							/** Just for test */
							SOCKADDR_IN  NewSockAddr = {0};
							TI32_t OutSize = sizeof(NewSockAddr);

							SOCKET s = accept(pBaseIPServer->Socket(), (SOCKADDR *)&NewSockAddr, (int *)&OutSize);
							if(s == INVALID_SOCKET)
							{
								if(pBaseIPServer->Log())
									pBaseIPServer->Log()->Log(NOCOMMENTS, "Accept Failure (%u)", WSAGetLastError());
							}
							else
							{
								/** Treat new Client */

								/** Wait For Another Client */
								SetEvent(pBaseIPServer->m_TabEvents[CBaseIPServer_Events_WaitForClientConnection]);
							}
						}
						break;
					default:
						break;
				}
			}
		}
	}
}
