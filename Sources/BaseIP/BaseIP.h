#ifndef CBASEIP_H
#define CBASEIP_H
#include "Common\MyTypes.h"
#include "BaseIP_Defs.h"
#if defined (WIN32) || defined (WINCE)
#include "WinSock.h"
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

class CLog;
class CMutex;
TEMPLAT_TYPE_DECL
class CMyString;

class CBaseIP
{
public:
	CBaseIP(BaseIPType_t IPConnectionType, TS08_t ConnectionString, CLog * pLog = NULL); /** XXX.XXX.XXX.XXX:XXXXX -> IP:Port */
	~CBaseIP();

	bool SetNewConnectionString(TS08_t ConnectionString);
	void RefreshLocalHostInfos();

	/** Inline Public Getters */
	inline const CMyString<char> & IP()const{return *m_pIPAdress;}
	inline const TU16_t Port()const{return m_Port;}
	inline const CMyString<char> & DomaineName()const{return *m_pDomaineName;}

protected:
	virtual bool CreateSocket() = NULL;
	bool CloseSocket();

	/** Inline Protected getters */
	inline SOCKADDR_IN * TargetAddrStructure(){return &m_TargetSockAddr;}
	inline SOCKADDR_IN * SourceAddrStructure(){return &m_ClientSockAddr;}
	inline SOCKET Socket()const{return m_Socket;}
	inline CMutex * SyncMutex()const {return m_pSyncMutex;}

	inline void SetSocket(SOCKET Sock){m_Socket = Sock;};
private:
	/** IP Adress */
	CMyString<char> * m_pDomaineName;
	CMyString<char> * m_pIPAdress;
	TU16_t m_Port;

	/** Sockets */
	SOCKADDR_IN m_TargetSockAddr;
	SOCKADDR_IN m_ClientSockAddr;
	SOCKET m_Socket;

	/** Logs */
	CLog * m_pLog;

	/** Socket Lock */
	CMutex * m_pSyncMutex;

	/** IP Conenction Type */
	BaseIPType_t m_IpConnectionType;
};
#endif