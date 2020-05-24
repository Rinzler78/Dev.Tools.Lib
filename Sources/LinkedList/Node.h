#ifndef CNODE_H
#define CNODE_H

#include "Common\MyTypes.h"

TEMPLAT_DECL
class CNode
{
public:
	CNode(T pData, CNode<T> * pParentNode = NULL, CNode<T> * pNextNode = NULL);
	~CNode();
	inline T Data()const {return m_pData;}
	inline CNode<T> * Parent()const {return m_pParentNode;}
	inline void SetNewParent(CNode<T> * pNewParentNode){m_pParentNode = pNewParentNode;}

	inline CNode<T> * Child()const {return m_pChildNode;}
	inline void SetNewChild(CNode<T> * pNewChildNode){m_pChildNode = pNewChildNode;}
	
private:
	T m_pData;  ///< The data
	CNode<T> * m_pChildNode; ///< The next node
	CNode<T> * m_pParentNode;
};
#include "Node.cpp"
#endif