#include "stdafx.h"
#include "PoolX.h"

PoolX::PoolX()
{
	m_pPool = NULL;
	m_nLimit = 0;
	m_nFill = 0;
}

PoolX::~PoolX()
{
	releaseAll(m_pPool);
	m_pPool = NULL;
}

Inlist* PoolX::get()
{
	if (m_pPool == NULL) {
		return createItem();
	}
	Inlist *pRet = m_pPool;
	m_pPool = m_pPool->next;
	pRet->next = NULL;
	return pRet;
}

bool PoolX::clearItem(Inlist* pItem) {
	return true;
}

Inlist* PoolX::release( Inlist* pList, Inlist* pItem )
{
	if (pItem == NULL) {
		return pList;
	}
	clearItem(pItem);

	return Inlist::remove(pList, pItem);
}

Inlist* PoolX::releaseAll( Inlist* pList )
{
	if (pList == NULL) {
		return NULL;
	}

	while (pList != NULL) {
		Inlist *pNext = pList->next;

		clearItem(pList);

		pList->next = m_pPool;
		m_pPool = pList;

		delete pList;

		pList = pNext;		
	}
	return NULL;
}

Inlist* PoolX::release( Inlist* pItem )
{
	if (pItem == NULL)
		return NULL;
	if (!clearItem(pItem))
		return NULL;
	pItem->next = m_pPool;
	m_pPool = pItem;

	return NULL;
}

