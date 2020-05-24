#ifndef CBASECLIENTCOMMUNICATION_H
#define CBASECLIENTCOMMUNICATION_H
#include "Common\MyTypes.h"
#include "Windows.h"

/** Connection Type */
typedef enum BaseClientConnectionType
{
	BaseClientConnectionType_Serial,
	BaseClientConnectionType_TCPClient,
	BaseClientConnectionType_UDPClient,
	MAX_BaseClientConnectionType
}BaseClientConnectionType_t;

class CThread;
class CMutex;
class CLog;
TEMPLAT_DECL
class CLinkedList;
class CFrame;
class CBaseClientCommunication_Delegate;

class CBaseClientCommunication
{
public:
	CBaseClientCommunication(BaseClientConnectionType_t ObjectType, CBaseClientCommunication_Delegate * pDelegate, CLog * pLog = NULL);
	~CBaseClientCommunication();

	/** Frame Reception */
	TU32_t Receive(TU08_t * pOutBuffer, TU32_t NbBytesWaited);
	bool ReceiveAsync(TU32_t NbBytesWaited);

	/** Frame Send */
	TU32_t Send(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier = 0);
	bool SendAsync(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier = 0);

	/** Log Object */
	inline CLog * LogObject()const{return m_pLog;}

	/** Data Exchange */
	inline TU32_t TotalNumberOfBytesSent() const {return m_TotalNumberOfBytesSent;}
	inline TU32_t TotalNumberOfBytesReceived() const {return m_TotalNumberOfBytesReceived;}

	/** Object Type */
	inline BaseClientConnectionType_t ObjectType()const { return m_ObjectType;}

protected:

	/** Set Delegate */
	inline void SetDelegate(CBaseClientCommunication_Delegate * pDelegate){m_pDelegate = pDelegate;};
	
	/** Get Delegate */
	inline CBaseClientCommunication_Delegate * Delegate()const {return m_pDelegate;}
	
	/** Send */
	virtual TU32_t VSendData(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier = NULL) = NULL; 
	
	/** Receive */
	virtual TU32_t VReceiveData(TU08_t * pOutBuffer, TU32_t outSize) = NULL;

	/** Action Received */
	virtual void ExecuteAction(TU32_t ActionID){};

	bool AddAsyncAction(TU32_t ActionID, TU32_t NbSecondToSleepAfter = 0, bool HighPriority = false);
private:
	
	/** Logs */
	CLog * m_pLog;

	/** Object Shared By CallBacks */
	void * m_pToObj;

	/** Reception Buffer */
	TU08_t * m_pReceptionBuffer;
	TU32_t m_ReceptionBufferSize;
	TU32_t m_SizeWaited;
	
	/** Action List */
	CLinkedList<TU32_t *> * m_pActionList;
	TU32_t GetFirstAction();
	void FreeActionList();
	CMutex * m_pActionListMutex;
	
	/** Asynchrone Actions */
	/** Kill */
	HANDLE m_KillEvent;
	CThread * m_pActionThread;
	friend void ActionThreadFunction(void * pParams);
	void OnTopEvent();
	HANDLE m_TopEvent;

	/** Async Mutex */
	CMutex * m_pReceptionMutex;
	CMutex * m_pSendMutex;
	CMutex * m_pAsyncSendMutex;
	CMutex * m_pSendListMutex;

	CLinkedList<CFrame *> * m_pSendFrameList;
	void SendAllFrames();
	void PurgeFrameList();

	/** Data Exchange */
	TU32_t m_TotalNumberOfBytesSent;
	TU32_t m_TotalNumberOfBytesReceived;

	/** Delegate */
	CBaseClientCommunication_Delegate * m_pDelegate;

	/** Object Type */
	BaseClientConnectionType_t m_ObjectType;
};
#endif