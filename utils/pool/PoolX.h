#pragma once
#include "Inlist.h"

class PoolX {

public:
	PoolX();
	virtual ~PoolX();

protected:
	Inlist* m_pPool;
	int m_nLimit;
	int m_nFill;

protected:
	virtual bool clearItem(Inlist* pItem);
	virtual Inlist* createItem() = 0;

public:
	Inlist* release(Inlist* pItem);
	Inlist* releaseAll(Inlist* pList);
	Inlist* release(Inlist* pList, Inlist* pItem);

	Inlist* get();

};

