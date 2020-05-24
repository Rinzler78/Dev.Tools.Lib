#ifndef CLINKEDLIST_CPP /** Implementation needed for Template */
#define CLINKEDLIST_CPP

#include "LinkedList\LinkedList.h"
#include "Node.h"
#include <stdarg.h>

#pragma region <Template Implementation>
TEMPLAT_DECL
CLinkedList<T>::CLinkedList() 
: m_pFirstNode(NULL)
, m_pLastNode(NULL)
, m_Count(0)
{
}
TEMPLAT_DECL
CLinkedList<T>::CLinkedList(T pT,...) 
: m_pFirstNode(NULL)
, m_pLastNode(NULL)
, m_Count(0)
{
	va_list vl;
	T Val = NULL;

	Add(pT);

	va_start(vl, pT);
	
	do
	{
		Val = va_arg(vl,T);
		if(Val)
			Add(Val);
	}

	while (Val != NULL);
	
	va_end( vl );
}
TEMPLAT_DECL
CLinkedList<T>::~CLinkedList()
{
	Clear();
}
TEMPLAT_DECL
void CLinkedList<T>::Clear()
{
	while(m_Count)
		DeleteAt(0);
}
TEMPLAT_DECL
bool CLinkedList<T>::InsertAt(TU32_t Index, T pT)
{
	bool Result = false;
	
	if(Index < m_Count)
	{
		/** Retreive Node AT Index */
		CNode<T> *pNode = GetNodeAt(Index);

		if(pNode)
		{	
			/** Create New Node With Parent node and Child Node */
			CNode<T> *pNewNode = new CNode<T>(pT, pNode->Parent(), pNode);
			if(pNode == m_pFirstNode)
				m_pFirstNode = pNewNode;
			
			/** Increase Element Count */
			++m_Count;
		}
	}
	else
		Result = Add(pT);
	return Result;
}
TEMPLAT_DECL
bool CLinkedList<T>::Add(T pT)
{
	bool Result = false;
	CNode<T> *pNode = NULL;

	if(!m_pFirstNode)
	{
		/** Add First Node */
		m_pFirstNode = new CNode<T>(pT);
		m_pLastNode = m_pFirstNode;
	}
	else
		m_pLastNode = new CNode<T>(pT, m_pLastNode);
	
	++m_Count;
	Result = true;

	return Result;
}
TEMPLAT_DECL
bool CLinkedList<T>::AddRange(CLinkedList<T> *pList)
{
	bool Result = false;
	
	if(pList)
	{
		for(TU32_t i = 0 ; i < pList->Count() ; ++i)
		{
			T Element = (*pList)[i];
			Result = Add(Element);
		}
	}

	return Result;
}
TEMPLAT_DECL
int CLinkedList<T>::FindFirstIndex(T pT) const
{
	int Result = -1;
	//TU32_t Index = 0;
	CNode<T> *pNode = NULL;

	if(m_pFirstNode != NULL)
	{
		/** Get First Node */
		pNode = m_pFirstNode;
		Result = 0;
		
		while(pNode->Data() != pT)
		{
			if(pNode->Child() == NULL)
			{
				Result = -1;
				break;
			}

			++Result;
			pNode = pNode->Child();
		}
	}

	return Result;
}
TEMPLAT_DECL
bool CLinkedList<T>::DeleteAt(TU32_t Index)
{
	bool Result = false;
	
	if(Index < m_Count)
	{
		/** Retreive Node AT Index */
		CNode<T> *pNode = GetNodeAt(Index);

		if(pNode)
		{
			if(pNode == m_pFirstNode)
			{
				m_pFirstNode = pNode->Child();
				if(m_pFirstNode)
					m_pFirstNode->SetNewParent(NULL);
				else
					m_pLastNode = NULL;
			}
			else if(pNode == m_pLastNode)
			{
				m_pLastNode = m_pLastNode->Parent();
				if(m_pLastNode == m_pFirstNode)
					m_pFirstNode->SetNewChild(NULL);
			}
			else
				if(pNode->Parent())
					pNode->Parent()->SetNewChild(pNode->Child());

			MACRO_DELET(pNode);
			--m_Count;
		}
	}
	return Result;
}
TEMPLAT_DECL
bool CLinkedList<T>::DeleteFirst()
{
	return DeleteAt(0);
}
TEMPLAT_DECL
bool CLinkedList<T>::Delete(T pT)
{
	bool Result = false;
	CNode<T> * pNode = NULL;
	
	if(pT)
	{
		pNode = m_pFirstNode;
		while((pNode->Data() != pT) )//&& (pNode->Child() != NULL))
			pNode = pNode->Child();

		if(pNode) /** Node founded */
		{
			if(pNode->Parent()) /** Node have a parent Node */
			{
				/** Link n + 1 to n - 1 */
				pNode->Parent()->SetNewChild(pNode->Child());
			}
			else if(pNode->Child())/** Have a child Node */
			{
				m_pFirstNode = pNode->Child();
			}
			else
			{
				/** Only On Node */
				m_pLastNode = NULL;
				m_pFirstNode = NULL;
			}
			
			MACRO_DELET(pNode);
			m_Count--;

			Result = true;
		}
	}
	return Result;
}
TEMPLAT_DECL
void CLinkedList<T>::operator += (T o1)
{
	Add(o1);
}
TEMPLAT_DECL
void CLinkedList<T>::operator -= (T o1)
{
	Delete(o1);
}
TEMPLAT_DECL
T CLinkedList<T>::operator[](TU32_t IndexSearch)const
{
	T Data = NULL;
	CNode<T> *pNode = NULL;
	TU32_t Index = 0;
	
	if(IndexSearch < m_Count)
	{
		CNode<T> *pNode = GetNodeAt(IndexSearch);
		
		if(pNode)
			Data = pNode->Data();
	}

	return Data;
}
TEMPLAT_DECL
CNode<T> * CLinkedList<T>::GetNodeAt(TU32_t IndexSearch)const
{
	CNode<T> *pNode = NULL;
	TU32_t Index = 0;
	
	if(IndexSearch < m_Count)
	{
		pNode = m_pFirstNode;

		while(Index != IndexSearch && pNode)
		{
			pNode = pNode->Child();
			++Index;
		}
	}

	return pNode;
}

#pragma endregion
#endif