#include "BaseClientCommunication\BaseClientCommunication.h"
#include "Windows.h"
#include "Mutex\Mutex.h"
#include "Thread\Thread.h"
#include "Log\Log.h"
#include "LinkedList\LinkedList.h"
#include "BaseClientCommunication\BaseClientCommunication_Defs.h"
#include "BaseClientCommunication\BaseClientCommunication_Delegate.h"
#include "LinkedList\LinkedList.h"

CBaseClientCommunication::CBaseClientCommunication(BaseClientConnectionType_t ObjectType, CBaseClientCommunication_Delegate * pDelegate, CLog * pLog)
:m_pLog(pLog)

/** Log */
, m_pToObj(NULL)

/** Reception */
, m_pReceptionBuffer(NULL)
, m_ReceptionBufferSize(0)
, m_TotalNumberOfBytesReceived(0)
, m_SizeWaited(0)

/** Async Action */
, m_TopEvent(CreateEvent(NULL,false,false,NULL))
, m_pActionList(new CLinkedList<TU32_t *>())
, m_pActionListMutex(new CMutex(L"Base Communication - Action List Mutex"))
, m_pReceptionMutex(new CMutex(L"Base Communication - Reception Mutex"))
, m_pSendMutex(new CMutex(L"Base Communication - Send Mutex"))
, m_pActionThread(new CThread("Base Communication Thread", ActionThreadFunction, m_pLog))
, m_KillEvent(INVALID_HANDLE_VALUE)
, m_pAsyncSendMutex(new CMutex(L"Base Communication - Send Async Mutex"))

/** Send */
, m_pSendFrameList(new CLinkedList<CFrame *>())
, m_TotalNumberOfBytesSent(0)
, m_pSendListMutex(new CMutex(L"Base Communication - Send List Mutex"))

/** Delegate */
, m_pDelegate(pDelegate)
, m_ObjectType(ObjectType)
{
	/** Thread Action init */
	m_pActionThread->Start(this);
}
CBaseClientCommunication::~CBaseClientCommunication()
{
	AddAsyncAction(BaseClientCommunication_Event_Exit, 0, true);
	/** Action thread Kill */
	MACRO_DELET(m_pActionThread)

	/** Action list Kill */
	FreeActionList();
	MACRO_DELET(m_pActionList)

	/** Free Send list */
	PurgeFrameList();
	MACRO_DELET(m_pSendFrameList)

	/** Kill Mutex */
	MACRO_DELET(m_pSendMutex)
	MACRO_DELET(m_pReceptionMutex)
	MACRO_DELET(m_pActionListMutex)
	MACRO_DELET(m_pAsyncSendMutex)
	MACRO_DELET(m_pSendListMutex)

	/** Reception Buffer Clean */
	MACRO_DELET_TAB(m_pReceptionBuffer)
}
void CBaseClientCommunication::FreeActionList()
{
	if(m_pActionList)
	{
		if(m_pActionListMutex)
			m_pActionListMutex->Wait();

		while(m_pActionList->Count())
		{
			TU32_t * pAction = (*m_pActionList)[0];
			MACRO_DELET(pAction)
			m_pActionList->DeleteFirst();
		}

		if(m_pActionListMutex)
			m_pActionListMutex->Release();
	}
}
TU32_t CBaseClientCommunication::GetFirstAction()
{
	TU32_t Result = BaseClientCommunication_Event_Init;
	
	if(m_pActionList)
	{
		if(m_pActionListMutex)
			m_pActionListMutex->Wait();
		
		TU32_t * pAction = (*m_pActionList)[0];
		if(pAction)
			Result = *pAction;
		
		m_pActionList->DeleteFirst();
		MACRO_DELET(pAction)

		if(m_pActionListMutex)
			m_pActionListMutex->Release();
	}
	return Result;
}
bool CBaseClientCommunication::AddAsyncAction(TU32_t ActionID, TU32_t NbSecondToSleepAfter, bool HighPriority)
{
	bool Result = false;
	TU32_t * pActionToPost = NULL;

	pActionToPost = new TU32_t;
	*pActionToPost = ActionID;
	
	if(m_pActionListMutex)
		m_pActionListMutex->Wait();
	
	if(!HighPriority)
		m_pActionList->Add(pActionToPost);
	else
		m_pActionList->InsertAt(0, pActionToPost);

	if(m_pActionListMutex)
		m_pActionListMutex->Release();

	Result = (SetEvent(m_TopEvent) == TRUE);

	if(ActionID == BaseClientCommunication_Event_Exit)
	{
		WaitForSingleObject(m_KillEvent, INFINITE);
	}

	return Result;
}

/** Frame Reception */
TU32_t CBaseClientCommunication::Receive(TU08_t * pOutBuffer, TU32_t NbBytesWaited)
{
	TU32_t Result = 0;
	TU32_t NbByteReceived = 0;
	TU32_t NbByteToReceive = 0;
	//TU32_t RetryReceiveCount = 3;
	
	if(m_pReceptionMutex)
		m_pReceptionMutex->Wait();

	if(pOutBuffer && (NbBytesWaited > 0))
	{
		do
		{
			NbByteToReceive = NbBytesWaited - Result;
			NbByteReceived = VReceiveData(pOutBuffer + Result, NbByteToReceive);
			
			if(NbByteReceived <= NbByteToReceive)
			{
				Result += NbByteReceived;
			
				if(m_pLog)
					m_pLog->Log(NOCOMMENTS
								, "Nb Byte To Receive : %u\nNb Byte Received : %u\nTotal Nb Byte Received : %u"
								, NbByteToReceive
								, NbByteReceived
								, Result);

				m_TotalNumberOfBytesReceived += NbByteReceived;

				if(m_pLog)
					m_pLog->Log(NOCOMMENTS, "Total Number Of Bytes Received on channel : %u", m_TotalNumberOfBytesReceived);
			}
		}
		while((Result && (Result < NbBytesWaited) && (NbByteReceived /*|| RetryReceiveCount*/)));
	}

	if(m_pReceptionMutex)
		m_pReceptionMutex->Release();

	return Result;
}
bool CBaseClientCommunication::ReceiveAsync(TU32_t NbBytesWaited)
{
	bool Result = false;

	if(NbBytesWaited > 0)
	{
		if(NbBytesWaited > m_ReceptionBufferSize)
			MACRO_DELET_TAB(m_pReceptionBuffer)

		if(!m_pReceptionBuffer)
		{
			m_pReceptionBuffer = new TU08_t[NbBytesWaited];
			m_ReceptionBufferSize = NbBytesWaited;
			MACRO_MEMSET(m_pReceptionBuffer, m_ReceptionBufferSize)
		}

		m_SizeWaited = NbBytesWaited;

		Result = AddAsyncAction(BaseClientCommunication_Event_Receive);
	}

	return Result;
}

/** Frame Send */
TU32_t CBaseClientCommunication::Send(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier)
{
	TU32_t Result = 0;
	TU32_t NbByteSent = 0;
	TU32_t NbByteToSend = 0;
	
	if(m_pSendMutex)
		m_pSendMutex->Wait();

	if(pSendBuffer && Size)
	{
		do
		{
			NbByteToSend = Size - Result;
			NbByteSent = VSendData(pSendBuffer + Result, NbByteToSend, FrameIdentifier);
			Result += NbByteSent;
			if(m_pLog)
				m_pLog->Log(NOCOMMENTS
							, "Nb Byte To Send : %u\nNb Byte sent : %u\nTotal Nb Byte Sent : %u"
							, NbByteToSend
							, NbByteSent
							, Result);

			m_TotalNumberOfBytesSent += NbByteSent;
			if(m_pLog)
				m_pLog->Log(NOCOMMENTS, "Total Number Of Bytes Sent on channel : %u", m_TotalNumberOfBytesSent);
		}
		while((Result < Size) && (NbByteSent));
	}
	if(m_pSendMutex)
		m_pSendMutex->Release();

	return Result;
}
bool CBaseClientCommunication::SendAsync(TU08_t * pSendBuffer, TU32_t Size, TU32_t FrameIdentifier)
{
	bool Result = false;
	
	if(m_pAsyncSendMutex)
		m_pAsyncSendMutex->Wait();

	if(pSendBuffer && Size)
	{
		/** Add Frame To List */
		CFrame * pFrameToSend = new CFrame(pSendBuffer, Size, FrameIdentifier);
		if(m_pSendFrameList)
		{
			m_pSendFrameList->Add(pFrameToSend);
			Result = AddAsyncAction(BaseClientCommunication_Event_Send);
		}
	}

	if(m_pAsyncSendMutex)
		m_pAsyncSendMutex->Release();

	return Result;
}

void CBaseClientCommunication::SendAllFrames()
{
	/** Send Frames */
	if(m_pSendListMutex)
		m_pSendListMutex->Wait();

	while(m_pSendFrameList->Count())
	{
		TU32_t NBBytesSent = 0;

		/** Retrieve First Frame */
		CFrame * pFrame = (*m_pSendFrameList)[0];
		
		/** Send Frame */
		if(pFrame)
		{
			NBBytesSent = Send(pFrame->FrameBuffer(), pFrame->FrameBufferSize(), pFrame->FrameIdentifier());
					
			if(m_pDelegate)
				m_pDelegate->Event_FrameSendResult(pFrame->FrameBuffer()
													, pFrame->FrameBufferSize()
													, NBBytesSent
													, pFrame->FrameIdentifier());
		}
		
		/** Free Memory */
		MACRO_DELET(pFrame)
		
		/** Delete Frame From List */
		m_pSendFrameList->DeleteFirst();
	}

	if(m_pSendListMutex)
		m_pSendListMutex->Release();
}
void CBaseClientCommunication::PurgeFrameList()
{
	if(m_pSendListMutex)
		m_pSendListMutex->Wait();

	while(m_pSendFrameList->Count())
	{
		CFrame * pFrame = (*m_pSendFrameList)[0];
		if(pFrame)
			MACRO_DELET(pFrame)

		m_pSendFrameList->DeleteFirst();
	}

	if(m_pSendListMutex)
		m_pSendListMutex->Release();
}
void CBaseClientCommunication::OnTopEvent()
{
	TU32_t Result = 0;
	
	Result = GetFirstAction();

	switch(Result)
	{
		case BaseClientCommunication_Event_Exit:
			FreeActionList();
			SetEvent(m_KillEvent);
		break;
		case BaseClientCommunication_Event_Send:
				SendAllFrames();
			break;
		case BaseClientCommunication_Event_Receive:
			{
				TU32_t SizeReaded = 0;

				SizeReaded = Receive(m_pReceptionBuffer, m_SizeWaited);
				if(SizeReaded)
				{
					/** Deliver Frame */
					if(m_pLog)
						m_pLog->Log(NOCOMMENTS, (SizeReaded > 1 ? "%u/%u Bytes received": "%u/%u Byte received"), SizeReaded, m_SizeWaited);
					
					if(m_pDelegate)
						m_pDelegate->Event_FrameReceived(m_pReceptionBuffer, SizeReaded, m_SizeWaited);

					MACRO_MEMSET(m_pReceptionBuffer,m_ReceptionBufferSize)
				}
				else
					AddAsyncAction(BaseClientCommunication_Event_Receive);
			}
			break;
		case BaseClientCommunication_Event_Sleep:
			break;
		default:
			ExecuteAction(Result);
			break;
	}
}
void ActionThreadFunction(void * pParams)
{
	DWORD Result = 0;
	if(pParams)
	{
		CThread * pThread = (CThread *)pParams;
		CBaseClientCommunication * pThis = (CBaseClientCommunication *)pThread->ThreadParam();
		
		while((pThread->State() == CThread_State_Running))
		{
			Result = WaitForSingleObject(pThis->m_TopEvent, INFINITE);
			Result = pThis->GetFirstAction();
			
			if(Result == BaseClientCommunication_Event_Exit)
				break;

			switch(Result)
			{
				case BaseClientCommunication_Event_Send:
						pThis->SendAllFrames();
					break;
				case BaseClientCommunication_Event_Receive:
					{
						TU32_t SizeReaded = 0;

						SizeReaded = pThis->Receive(pThis->m_pReceptionBuffer, pThis->m_SizeWaited);
						if(SizeReaded)
						{
							/** Deliver Frame */
							if(pThis->m_pLog)
								pThis->m_pLog->Log(NOCOMMENTS, (SizeReaded > 1 ? "%u/%u Bytes received": "%u/%u Byte received"), SizeReaded, pThis->m_SizeWaited);
							
							if(pThis->m_pDelegate)
								pThis->m_pDelegate->Event_FrameReceived(pThis->m_pReceptionBuffer, SizeReaded, pThis->m_SizeWaited);

							MACRO_MEMSET(pThis->m_pReceptionBuffer, pThis->m_ReceptionBufferSize)
						}
						else
							pThis->AddAsyncAction(BaseClientCommunication_Event_Receive);
					}
					break;
				case BaseClientCommunication_Event_Sleep:
					break;
				default:
					pThis->ExecuteAction(Result);
					break;
			}

			if(pThis->m_pActionList->Count() > 0)
				SetEvent(pThis->m_TopEvent);

			Sleep(100);
		}

		pThis->FreeActionList();
		SetEvent(pThis->m_KillEvent);
	}
}