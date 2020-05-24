#ifndef CLINKED_LIST_H
#define CLINKED_LIST_H
#include "Common\MyTypes.h"

TEMPLAT_DECL
class CNode;

TEMPLAT_DECL
class CLinkedList
{
public:
	CLinkedList();
	CLinkedList(T pT,...);
	~CLinkedList();

	/** Append */
	bool InsertAt(TU32_t Index, T pT);
	bool Add(T pT);
	bool AddRange(CLinkedList<T> *pList);
	void operator += (T o1);

	/** Delete */
	bool DeleteAt(TU32_t Index);
	bool DeleteFirst();
	bool Delete(T pT);
	void Clear();
	void operator -= (T o1);

	/** Getters */ 
	inline T GetLast() const{return (m_pLastNode ? m_pLastNode->Data():NULL);};
	int FindFirstIndex(T pT) const; /** Not Found -> -1 */
	inline const TU32_t Count()const{return m_Count;}
	T operator[](TU32_t Index)const;
	
private:
	CNode<T> * GetNodeAt(TU32_t Index)const;
	TU32_t m_Count;   ///< Number of 
	CNode<T> *m_pFirstNode; ///< The first node
	CNode<T> *m_pLastNode;  ///< The last node
};
#include "LinkedList.cpp"
#endif