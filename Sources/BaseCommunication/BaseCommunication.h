#ifndef CBASECOMMUNICATION_H
#define CBASECOMMUNICATION_H
#include "Common\Mytypes.h"
#include "BaseCommunication_Defs.h"
#include "BaseCommunication_Delegate.h"
#include "Windows.h"

class CThread;
class CLog;
class CMutex;
TEMPLAT_DECL
class CLinkedList;

class CBaseCommunication : public CBaseCommunicationDelegate
{
public:
	CBaseCommunication(CBaseCommunicationDelegate * pDelegate, CLog * pLog = NULL);
	~CBaseCommunication();

	/** Frame Reception */
	TU32_t Receive(TU08_t * pOutBuffer, TU32_t NbBytesWaited);
	bool ReceiveAsync(TU32_t NbBytesWaited);

	/** Frame Send */
	TU32_t Send(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier = 0);
	bool SendAsync(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier = 0);

protected:
	/** Send */
	virtual TU32_t VSend(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier = NULL) = NULL; 
	/** Receive */
	virtual TU32_t VReceive(TU08_t * pOutBuffer, TU32_t outSize) = NULL;
private:
	/** Logs */
	CLog * m_pLog;

	/** Delegate */
	CBaseCommunicationDelegate * m_pDelegate;
	
	/** Reception Buffer Size */
	TU32_t m_NbBytesWaited;

	/** Asynchron Actions */
	CThread * m_pActionThread;
	
	HANDLE m_TopEvent;
	CLinkedList<BaseClient_Event_t *> * m_pActionList;
	CMutex * m_pActionListMutex;

	friend void ActionThreadFunction(void * pParams);

	/** Async Mutex */
	CMutex * m_pReceptionMutex;
	CMutex * m_pSendMutex;
	CMutex * m_pAsyncMutex;
	CMutex * m_pConnectionMutex;
	CLinkedList<CFrame *> * m_pSendFrameList;

	/** Kill */
	HANDLE m_KillEvent;

	/** Data Exchange */
	TU32_t m_TotalNumberOfBytesSent;
	TU32_t m_TotalNumberOfBytesReceived;
};
#endif