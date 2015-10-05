#pragma once
#include "Inlist.h"



class SyncPool {

protected:
	int m_nMaxFill;
	bool m_bClearItems;

	int m_nFill;
	Inlist *m_pPool;

public:
	SyncPool(int maxItemsInPool, bool clearItem = false);
	virtual ~SyncPool();
	
public:
	int getFill();
	
	void clear();

	virtual Inlist* get();

	virtual Inlist* release(Inlist *pItem);
	virtual Inlist* releaseAll(Inlist *pItem);

public:
	virtual Inlist* createItem() = 0;
	virtual void init(int items);
	virtual bool clearItem(Inlist* pItem);
	virtual void freeItem(Inlist* pItem);

};