#if defined (WIN32) || defined (WINCE)
#include <winsock2.h>
#if defined (UNDER_CE)
#pragma comment(lib, "Ws2.lib")
#else
#pragma comment(lib, "ws2_32.lib")
#endif
#endif

#include "BaseTCPClient.h"
#include "MyString\MyString.h"
#include "Log\Log.h"
#include "Mutex\Mutex.h"

CBaseTCPClient::CBaseTCPClient(CBaseConnectedClient_Delegate * pDelegate, TS08_t ConnectionString, CLog * pLog)
:CBaseIP(BaseIPType_TCP_Client, ConnectionString, pLog)
, CBaseConnectedClient(BaseClientConnectionType_TCPClient, pDelegate, pLog)
{
}
CBaseTCPClient::~CBaseTCPClient()
{
	//AddAsyncAction(BaseClient_Event_Exit, true);
}
bool CBaseTCPClient::CreateSocket()
{
	bool Result = false;
	
	SyncMutex()->Wait();
	if(Socket() == INVALID_SOCKET)
	{
		SetSocket(socket(TargetAddrStructure()->sin_family, SOCK_STREAM,IPPROTO_IP));
		
		Result = (Socket() != INVALID_SOCKET);
	}
	SyncMutex()->Release();

	return Result;
}
bool CBaseTCPClient::VWillConnect()
{
	return true;
}
bool CBaseTCPClient::VConnect()
{
	bool Result = false;
	TU32_t LastError = 0;

	if(!Connected())
	{
		if(Socket() == INVALID_SOCKET)
			CreateSocket();

		if(Socket() != INVALID_SOCKET)
		{
			if(LogObject())
				LogObject()->Log(NOCOMMENTS, "Try to connect to %s (%s) on port %u", DomaineName().String(), IP().String(), Port());

			/** Try Connect */
#if defined (WIN32) || defined (WINCE)
			LastError = connect(Socket(), (const sockaddr *)TargetAddrStructure(), sizeof(*(TargetAddrStructure())));
#else
			LastError = connect(Socket(), (const SOCKADDR_IN *)TargetAddrStructure(), sizeof(*(TargetAddrStructure())));
#endif
			if(LastError == SOCKET_ERROR)
			{
				switch(LastError = WSAGetLastError())
				{
					case WSAECONNREFUSED:
						if(LogObject())
							LogObject()->Log(NOCOMMENTS, "Connection refused by remote host");
						break;
					default:
						break;
				}
			}
			else
				Result = true;
		}
	}

	return Result;
}
bool CBaseTCPClient::VDidConnect()
{
	return true;
}
bool CBaseTCPClient::VWillDisConnect()
{
	return true;
}
bool CBaseTCPClient::VDisConnect()
{
	bool Result = false;

	if(Connected())
	{
		if(LogObject())
			LogObject()->Log(NOCOMMENTS, "Try to disconnect from %s (%s)", DomaineName().String(), IP().String());

		Result = CloseSocket();
	}

	return Result;
}
bool CBaseTCPClient::VDidDisConnect()
{
	return true;
}
TU32_t CBaseTCPClient::VSend(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier)
{
	TU32_t Result = 0;
	TI32_t LastError = 0;

	if(Connected())
	{
		Result = send(Socket(), (const TC08_t *)pSendBuffer, Size, 0);
		if(Result == SOCKET_ERROR)
		{
			Result = 0;
			switch(LastError = WSAGetLastError())
			{
				case WSAEOPNOTSUPP:
					if(LogObject())
						LogObject()->Log(NOCOMMENTS,"Operation Not supported (Error : %u)", LastError);
					DisConnectAsync();
					break;
			}
		}
	}

	return Result;
}
TU32_t CBaseTCPClient::VReceive(TU08_t * pOutBuffer, TU32_t outSize)
{
	TU32_t Result = 0;
	TI32_t LastError = 0;

	if(Connected())
	{
		Result = recv(	Socket()
						, (TC08_t *)pOutBuffer
						, outSize
#if defined (UNDER_CE)
						, MSG_PARTIAL
#else
						, MSG_WAITALL
#endif
						);

		if(Result == SOCKET_ERROR)
		{
			Result = 0;
			switch(LastError = WSAGetLastError())
			{
				case WSAEOPNOTSUPP:
					if(LogObject())
						LogObject()->Log(NOCOMMENTS,"Operation Not supported (Error : %u)", LastError);
					DisConnectAsync();
					break;
			}
		}
	}

	return Result;
}