#if defined (WIN32) || defined (WINCE)
#include <winsock2.h>
#endif

#include "BaseIp\BaseIp.h"
#include "Log\Log.h"
#include "Mutex\Mutex.h"
#include "CommonTools\CommonTools.h"
#include "String.h"
#include "stdio.h"
#include "CType.h"
#include "stdlib.h"
#include "MyString\MyString.h"

#if defined (WIN32) || defined (WINCE)
#if defined (UNDER_CE)
#pragma comment(lib, "Ws2.lib")
#else
#pragma comment(lib, "ws2_32.lib")
#endif
#endif

CBaseIP::CBaseIP(BaseIPType_t IPConnectionType, TS08_t ConnectionString, CLog * pLog) /** XXX.XXX.XXX.XXX:XXXXX -> IP:Port */
:m_Socket(INVALID_SOCKET)
, m_Port(0)
, m_pLog(NULL)
, m_pSyncMutex(new CMutex(L"Base Ip Mutex"))
, m_IpConnectionType(IPConnectionType)
, m_pDomaineName(new CMyString<char>())
, m_pIPAdress(new CMyString<char>())
{
#if defined (WIN32) || defined (WINCE)
	/** WSA Startup */
	WSADATA WSAData;
	if(WSAStartup(MAKEWORD(2,2), &WSAData) < 0)
		if(m_pLog)m_pLog->Log(NOCOMMENTS, "WSAStartup failure");
#endif
	
	MACRO_MEMSET(&m_TargetSockAddr, sizeof(m_TargetSockAddr))
	MACRO_MEMSET(&m_ClientSockAddr, sizeof(m_ClientSockAddr))
	/** Create Socket */
	SetNewConnectionString(ConnectionString);
	
	/** Get Local Host Infos */
	RefreshLocalHostInfos();
}
CBaseIP::~CBaseIP()
{
	CloseSocket();
#if defined (WIN32) || defined (WINCE)
	/** Clean WSA */
	WSACleanup();
#endif
	MACRO_DELET(m_pDomaineName)
	MACRO_DELET(m_pIPAdress)
	MACRO_DELET(m_pSyncMutex)
}
bool CBaseIP::SetNewConnectionString(TS08_t pConnectionString)
{
	bool Result = false;
	TU32_t Index = 0;
	const TC08_t Separator_Port = ':';

	if(pConnectionString)
	{
		CMyString<char> ConnectionString(pConnectionString);
		
		/** Search for ':' */
		Index = ConnectionString.FindFirst(Separator_Port);
		if(Index)
		{
			/** Init SockAddr Structure */
			MACRO_MEMSET(&m_TargetSockAddr, sizeof(m_TargetSockAddr))

			if(isalpha(ConnectionString[0]))
			{
				/** Retrieve Ip Adress from name */
				m_pDomaineName->SetString(Index, pConnectionString);
				struct hostent * pHost = gethostbyname(m_pDomaineName->String());

				/** Assuming to take first adress in list */
				if(pHost)
				{
#if defined (WIN32) || defined (WINCE)
				m_TargetSockAddr.sin_addr.S_un.S_addr = *(u_long *) pHost->h_addr_list[0];
				m_pIPAdress->SetString("%u.%u.%u.%u"	, m_TargetSockAddr.sin_addr.S_un.S_un_b.s_b1
														, m_TargetSockAddr.sin_addr.S_un.S_un_b.s_b2
														, m_TargetSockAddr.sin_addr.S_un.S_un_b.s_b3
														, m_TargetSockAddr.sin_addr.S_un.S_un_b.s_b4);
#else
				m_TargetSockAddr.sin_addr.s_addr = (__TU32_t)*(u_long *) pHost->h_addr_list[0];
				
				m_pIPAdress->SetString("%u.%u.%u.%u"	, (&m_TargetSockAddr.sin_addr.s_addr)[0]
														, (&m_TargetSockAddr.sin_addr.s_addr)[1]
														, (&m_TargetSockAddr.sin_addr.s_addr)[2]
														, (&m_TargetSockAddr.sin_addr.s_addr)[3]);
#endif
				}
			}
			else
			{
				/** Get Ip Adress String */
				m_pIPAdress->SetString(Index, pConnectionString);
				m_TargetSockAddr.sin_addr.s_addr = inet_addr(m_pIPAdress->String());

				/** Retrieve host name from IP */
				struct hostent * pHost = gethostbyaddr((const char *)&m_TargetSockAddr.sin_addr.s_addr, 4, AF_INET);
				if(pHost)
					(*m_pDomaineName) = pHost->h_name;
			}
			
			m_TargetSockAddr.sin_family = AF_INET;

			/** Get Port Number */
			m_Port = atoi(pConnectionString + Index + 1);
			m_TargetSockAddr.sin_port = htons(m_Port);

			Result = true;
		}
	}

	return Result;
}
bool CBaseIP::CloseSocket()
{
	bool Result = false;
	
	m_pSyncMutex->Wait();
	if(m_Socket != INVALID_SOCKET)
	{
		Result = (closesocket(m_Socket) == 0);
		m_Socket = INVALID_SOCKET;
	}
	m_pSyncMutex->Release();

	return Result;
}
void CBaseIP::RefreshLocalHostInfos()
{
	struct hostent * pHost = gethostbyname("localhost");

	if(pHost)
	{
		pHost = gethostbyname(pHost->h_name);
		if(pHost)
		{
			MACRO_MEMSET(&m_ClientSockAddr, sizeof(m_ClientSockAddr))
			m_ClientSockAddr.sin_family = AF_INET;
			m_ClientSockAddr.sin_addr.S_un.S_addr = *(u_long *) pHost->h_addr_list[0];	
		}
	}
}
