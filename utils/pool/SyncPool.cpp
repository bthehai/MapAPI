#include "stdafx.h"
#include "SyncPool.h"

SyncPool::SyncPool(int maxItemsInPool, bool clearItem /* = false */) 
	: m_nMaxFill(maxItemsInPool)
	, m_nFill(0)
	, m_bClearItems(clearItem)
	, m_pPool(NULL)
{
	
}

SyncPool::~SyncPool() { 

}

int SyncPool::getFill() {
	return m_nFill;
}

void SyncPool::clear() {
	Inlist *pTemp;
	while (m_pPool != NULL) {
		freeItem(m_pPool);
		pTemp = m_pPool;

		m_pPool = m_pPool->next;

		delete pTemp;
		pTemp = NULL;
	}
}

Inlist* SyncPool::get() {
	if (m_pPool == NULL) {
		return createItem();
	}
	m_nFill--;

	Inlist *pRet = m_pPool;
	m_pPool = m_pPool->next;
	pRet->next = NULL;

	return pRet;
}

Inlist* SyncPool::release(Inlist *pItem) {
	if (pItem == NULL) {
		return NULL;
	}
	if (m_bClearItems && !clearItem(pItem)) {
		freeItem(pItem);		
		return NULL;
	}
	if (m_nFill < m_nMaxFill) {
		m_nFill++;
		pItem->next = m_pPool;
		m_pPool = pItem;
	} else if (m_bClearItems) {
		freeItem(pItem);
	}
	return NULL;
}

Inlist* SyncPool::releaseAll(Inlist *pItem) {
	if (pItem == NULL) {
		return NULL;
	}
	if (m_nFill > m_nMaxFill) {
		while (pItem != NULL) {
			if (m_bClearItems) {
				clearItem(pItem);
				freeItem(pItem);
			}
			pItem = pItem->next;
		}
		return NULL;
	}

	while (pItem != NULL) {
		Inlist *pNext = pItem->next;
		if (m_bClearItems && !clearItem(pItem)) {
			freeItem(pItem);
			pItem = pNext;
			continue;
		}

		m_nFill++;

		pItem->next = m_pPool;
		m_pPool = pItem;

		pItem = pNext;
	}
	
	return NULL;
}

void SyncPool::init(int items) {
	m_nFill = 0;
	m_pPool = NULL;
}

bool SyncPool::clearItem(Inlist* pItem) {
	return true;
}

void SyncPool::freeItem(Inlist* pItem) {

}