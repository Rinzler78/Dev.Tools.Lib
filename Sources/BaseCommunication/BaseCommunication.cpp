#include "BaseCommunication.h"
#include "Thread\Thread.h"
#include "Mutex\Mutex.h"
#include "LinkedList\LinkedList.h"

CBaseCommunication::CBaseCommunication(CBaseCommunicationDelegate * pDelegate, CLog * pLog)
:m_pSendFrameList(NULL)
, m_pLog(pLog)
, m_pDelegate(pDelegate)
, m_NbBytesWaited(0)
, m_pActionThread(NULL)
, m_TopEvent(INVALID_HANDLE_VALUE)
, m_pActionList(NULL)
, m_pActionListMutex(NULL)
, m_pReceptionMutex(NULL)
, m_pSendMutex(NULL)
, m_pAsyncMutex(NULL)
, m_pConnectionMutex(NULL)
, m_KillEvent(INVALID_HANDLE_VALUE)
, m_TotalNumberOfBytesSent(0)
, m_TotalNumberOfBytesReceived(0)
{
}
CBaseCommunication::~CBaseCommunication()
{
}

/** Frame Reception */
TU32_t CBaseCommunication::Receive(TU08_t * pOutBuffer, TU32_t NbBytesWaited)
{
	if(!pOutBuffer || !NbBytesWaited)
		return 0;

	return 0;
}
bool CBaseCommunication::ReceiveAsync(TU32_t NbBytesWaited)
{
	if(!NbBytesWaited)
		return false;

	return false;
}

/** Frame Send */
TU32_t CBaseCommunication::Send(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier)
{
	if(!pSendBuffer || !Size)
		return 0;

	return 0;
}
bool CBaseCommunication::SendAsync(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifie)
{
	if(!pSendBuffer || !Size)
		return false;

	return false;
}