#ifndef CBASEIPSERVER_H
#define CBASEIPSERVER_H
#include "BaseIPServer_Defs.h"
#include "Common\MyTypes.h"
//#include "windows.h"
#include "BaseIp\BaseIp.h"

#define DEFAULT_NB_MAX_CLIENT_AT_THE_SAME_TIME 100

class CLog;
class CThread;
class CMutex;

class CBaseIPServer : public CBaseIP
{
public:
	CBaseIPServer(TS08_t ConnectionString, TU16_t NbMaxClientAtTheSameTime = DEFAULT_NB_MAX_CLIENT_AT_THE_SAME_TIME, CLog * pLog = NULL); /** XXX.XXX.XXX.XXX:XXXXX -> IP:Port */
	~CBaseIPServer();
	
	/** Start listening on specified port */
	bool Start();
	
	/** Stop Listening */
	bool Stop();

	/** Server State */
	inline CBaseIPServer_ServerState_t State()const{return m_ServerState;}

	/** Log */
	inline CLog * Log()const{return m_pLog;}

private:
	bool CreateSocket();
	/** Server Thread */
	CMutex * m_SyncMutex;
	CThread * m_pServerThread;
	friend void ServerThreadFunction(void * pParams);

	/** Events */
	HANDLE m_TabEvents[MAX_CBaseIPServer_Events];

	/** Log */
	CLog * m_pLog;

	/** Clients Connected */
	TU16_t m_NbMaxClientAtTheSameTime;

	/** Server State */
	CBaseIPServer_ServerState_t m_ServerState;

};
#endif