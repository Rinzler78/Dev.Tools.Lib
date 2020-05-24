#ifndef CNODE_CPP /** Implementation needed for Template */
#define CNODE_CPP
#include "Node.h"

TEMPLAT_DECL
CNode<T>::CNode(T pData, CNode<T> * pParentNode, CNode<T> *pNextNode) : 
m_pData(pData)
, m_pChildNode(pNextNode)
, m_pParentNode(pParentNode)
{
	if(pParentNode)
		pParentNode->SetNewChild(this);
	if(pNextNode)
		pNextNode->SetNewParent(this);
}
TEMPLAT_DECL
CNode<T>::~CNode()
{
	m_pData = NULL;
	m_pChildNode = NULL;
	m_pParentNode = NULL;
}
#endif