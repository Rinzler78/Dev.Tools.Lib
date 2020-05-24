#include "BaseUDPClient.h"
#include "Mutex\Mutex.h"
#if defined (WIN32) || defined (WINCE)
//#include <winsock2.h>
#if defined (UNDER_CE)
#pragma comment(lib, "Ws2.lib")
#else
#pragma comment(lib, "ws2_32.lib")
#endif
#endif

CBaseUDPClient::CBaseUDPClient(CBaseClientCommunication_Delegate * pDelegate, TS08_t ConnectionString, CLog * pLog)
:CBaseIP(BaseIPType_UDP_Client, ConnectionString, pLog)
, CBaseClientCommunication(BaseClientConnectionType_UDPClient, pDelegate, pLog)
{
	CreateSocket();
}
CBaseUDPClient::~CBaseUDPClient()
{
}
bool CBaseUDPClient::CreateSocket()
{
	bool Result = false;
	
	SyncMutex()->Wait();
	if(Socket() == INVALID_SOCKET)
	{
		SetSocket(socket(TargetAddrStructure()->sin_family, SOCK_DGRAM,IPPROTO_IP));
		TU16_t timeout_for_windows = 1000;
		//setsockopt(Socket(), SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout_for_windows, sizeof(timeout_for_windows));

		Result = (Socket() != INVALID_SOCKET);
	}
	SyncMutex()->Release();

	return Result;
}
TU32_t CBaseUDPClient::VSendData(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier)
{
	if(!pSendBuffer || !Size)
		return 0;
	TU32_t Ret = 0;
	Ret = sendto(	Socket()
					, (const TS08_t)pSendBuffer
					, Size
					, 0
					, (struct sockaddr *)TargetAddrStructure()
					, sizeof(struct sockaddr_in));

	return Ret;
}
TU32_t CBaseUDPClient::VReceiveData(TU08_t * pOutBuffer, TU32_t outSize)
{
	if(!pOutBuffer || !outSize)
		return 0;
	
	TU32_t Ret = 0;
	TU32_t LastErr = 0;
	struct sockaddr From = {0};
	TU32_t FromSize = sizeof(struct sockaddr);
	
	setsockopt(Socket(), SOL_SOCKET, SO_RCVTIMEO, (const char *)&outSize, sizeof(outSize));
	setsockopt(Socket(), SOL_SOCKET, SO_RCVBUF, (const char *)&outSize, sizeof(outSize));

	Ret = recvfrom(	Socket()
					, (TS08_t)pOutBuffer
					, outSize
					, 0
					, (struct sockaddr*)&From
					, (int *)&FromSize
					);
	
	LastErr = WSAGetLastError();
//#if defined (_DEBUG)
//	if(Ret == SOCKET_ERROR)
//		printf("recvfrom error : %u\n", LastErr);
//#endif

	if(Ret != SOCKET_ERROR)
		return Ret;

	if((LastErr == WSAEMSGSIZE))
		return outSize;

	return 0;

}