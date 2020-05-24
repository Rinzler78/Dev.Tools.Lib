#ifndef CBASECLIENT_H
#define CBASECLIENT_H
#include "BaseClient_Defs.h"
#include "BaseClientDelegate.h"
//#include "windows.h"
#include "LinkedList\LinkedList.h"
#include "Clock\Clock.h"

class CThread;
class CLog;
class CMutex;

class CBaseClient : protected CBaseClientDelegate
{
public:
	CBaseClient(ConnectionType_t Type
				, CBaseClientDelegate * pDelegate
				, CLog * pLog = NULL);
	~CBaseClient();
	
	/** Connection to target */
	bool Connect();
	bool ConnectAsync();

	/** Disconnection from target */
	bool DisConnect();
	bool DisConnectAsync();

	/** Reconnection */
	bool Reconnect();
	bool ReconnectAsync();

	/** Frame Reception */
	TU32_t Receive(TU08_t * pOutBuffer, TU32_t NbBytesWaited);
	bool ReceiveAsync(TU32_t NbBytesWaited);

	/** Frame Send */
	TU32_t Send(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier = 0);
	bool SendAsync(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier = 0);
	
	/** Connection State */
	inline ClientConnectionState_t ConnectionState() const{return m_ConnectionState;}

	/** Connection Type */
	inline ConnectionType_t ConnectionType() const {return m_ConnectionType;}
	inline bool Connected() const {return m_Connected;}
	/** Log Object */
	inline CLog * LogObject()const{return m_pLog;}

	/** Object Shared */
	virtual void SetDelegate(CBaseClientDelegate * pDelegate) = NULL;

	/** Data Exchange */
	inline TU32_t TotalNumberOfBytesSent() const {return m_TotalNumberOfBytesSent;}
	inline TU32_t TotalNumberOfBytesReceived() const {return m_TotalNumberOfBytesReceived;}

protected:
	/** Virtual Functions */
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

#if defined (FIFO_IMPLEMENTATION)
	bool AddAsyncAction(BaseClient_Event_t Action, TU32_t NbSecondToSleepAfter = 0, bool HighPriority = false);
	BaseClient_Event_t GetFirstAsyncAction();
#endif

private:
	/** Logs */
	CLog * m_pLog;
	void SetConnectionState(ClientConnectionState_t ConnexionState, CClock * pElapsedTime);

	/** Connection type */
	ConnectionType_t m_ConnectionType;

	/** Object Shared By CallBacks */
	void * m_pToObj;

	/** Connection State */
	ClientConnectionState_t m_ConnectionState;
	bool m_Connected;
	bool m_WaitForExit;
	
	/** Delegate */
	CBaseClientDelegate * m_pDelegate;
	
	/** Reception Buffer Size */
	TU32_t m_NbBytesWaited;

	/** Times */
	CClock m_TimeToConnect;
	CClock m_TimeToDisConnect;
	CClock m_TimeToReceive;
	CClock m_TimeToSend;

	/** Asynchron Actions */
	CThread * m_pActionThread;
#if defined (FIFO_IMPLEMENTATION)
	HANDLE m_TopEvent;
	CLinkedList<BaseClient_Event_t *> m_ActionList;
	CMutex * m_ActionListMutex;
#else
	HANDLE m_TabEvents[MAX_BaseClient_Event];
#endif
	friend void ActionThreadFunction(void * pParams);

	/** Async Mutex */
	CMutex * m_ReceptionMutex;
	CMutex * m_SendMutex;
	CMutex * m_AsyncMutex;
	CMutex * m_ConnectionMutex;
	CLinkedList<CFrame *> m_SendFrameList;

	/** Kill */
	HANDLE m_KillEvent;

	/** Data Exchange */
	TU32_t m_TotalNumberOfBytesSent;
	TU32_t m_TotalNumberOfBytesReceived;
};
#endif