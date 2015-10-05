#include "stdafx.h"
#include "Inlist.h"
#include <iostream>
#include "..\Log.h"


int Inlist::size(Inlist *pList) {
	int count = 0;
	for (Inlist* it = pList; it != NULL; it = it->next) {
		count++;
	}
	return count;
}


Inlist* Inlist::push(Inlist *pList, Inlist *pItem) {
	pItem->next = pList;
	return pItem;
}


Inlist* Inlist::remove(Inlist *pList, Inlist *pItem) {
	if (pItem == pList) {
		Inlist* head = pItem->next;
		((Inlist *)pItem)->next = NULL;
		return head;
	}

	Inlist *prev = pList;
	Inlist *it = pList->next;
	for (; it != NULL; it = it->next) {
		if (it == pItem) {
			prev->next = pItem->next;
			pItem->next = NULL;
			return pList;
		}
		prev = it;
	}

	return pList;
}


Inlist* Inlist::get(Inlist *pList, int i) {
	if (i < 0)
		return NULL;

	while (--i > 0 && pList != NULL)
		pList = pList->next;

	if (i == 0)
		return pList;

	return NULL;
}


Inlist* Inlist::appendItem(Inlist *pList, Inlist *pItem) {
	if (pList == NULL)
		return pItem;
	Inlist *it = pList;
	while (it->next != NULL) {
		it = it->next;
	}
	it->next = pItem;
	return pList;

}


Inlist* Inlist::appendList(Inlist *pList, Inlist *pOther) {
	if (pList == NULL)
		return pOther;
	if (pOther == NULL)
		return pList;

	Inlist *it = pList;
	while (it->next != NULL) {
		it = it->next;
	}
	it->next = pOther;
	return pList;
}


Inlist* Inlist::last(Inlist *pList) {
	Inlist *it = pList;
	while (it->next != NULL) {
		if (it->next == NULL)
			return it;
		it = it->next;
	}
	return NULL;
}


Inlist* Inlist::prependRelative(Inlist *pList, Inlist *pItem, Inlist *pOther) {
	if (pList == NULL) {		
		//Log::e("Inlist - prependRelative", "'list' is null");
		return NULL;
	}

	if (pList == pOther) {
		pItem->next = pList;
		return pItem;
	}

	Inlist *it = pList;
	while (it->next != NULL && it->next != pOther) {
		it = it->next;
	}

	if (it == NULL) {
		//Log::e("Inlist - prependRelative", "'other' is not in 'list'");
		return NULL;
	}

	pItem->next = it->next;
	it->next = pItem;

	return pList;
}

/************************************************************************/
/* List                                                                 */
/************************************************************************/

List::List() 
	: pHead(NULL)
	, pCur(NULL)
{
}

List::~List() {
	
}

void List::push(Inlist *pItem) {
	if (pItem->next == NULL) {
		//Log::e("List - push", "item.next must be null");
		return;
	}
	pItem->next = pHead;
	pHead = pItem;
}

Inlist* List::pop() {
	if (pHead == NULL) 
		return NULL;
	Inlist *pItem = pHead;
	pHead = pItem->next;
	pItem->next = NULL;
	return pItem;
}

void List::reverse() {
	Inlist *pTemp;
	Inlist *pItr = pHead;
	pHead = NULL;
	while (pItr != NULL) {
		pTemp = pItr->next;
		pItr->next = pHead;
		pHead = pItr;
		pItr = pTemp;
	}
}

void List::append(Inlist *pItem) {
	pHead = Inlist::appendItem(pHead, pItem);
}

void List::appendList(Inlist *pList) {
	pHead = Inlist::appendList(pHead, pList);
}

void List::remove(Inlist *pItem) {
	pCur = NULL;
	pHead = Inlist::remove(pHead, pItem);
}

Inlist* List::clear() {
	Inlist *pRet = pHead;
	pHead = NULL;
	pCur = NULL;
	return pRet;
}

Inlist* List::head() {
	return pHead;
}

int List::size() {
	return Inlist::size(pHead);
}

bool List::hasNext() {
	return pCur != NULL;
}

Inlist* List::next() {
	if (pCur == NULL) {
		//Log::e("", "");
		return NULL;
	}
	Inlist *pTemp = pCur;
	pCur = pCur->next;
	return pTemp;
}

void List::remove() {
	if (pHead->next == pCur) {
		pHead = pHead->next;
		return;
	}

	Inlist *pPrev = pHead;
	while (pPrev->next->next != pCur) {
		pPrev = pPrev->next;
	}
	pPrev->next = pCur;
}

Iterator<Inlist*>* List::iterator() {
	pCur = pHead;
	return this;
}