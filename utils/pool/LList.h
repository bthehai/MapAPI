#pragma once


template<class T>
class LList : public Inlist {

public:
	LList(T l);
	virtual ~LList();

public:
	T data;

public:
	static LList* find(LList *pList, T item);
	static LList* remove(LList *pList, T item);
	static LList* push(LList *pList, LList *pItem);
};


template<class T>
LList<T>::LList( T l )
{
	data = l;
}

template<class T>
LList<T>::~LList() {

}

template<class T>
LList<T>* LList<T>::find( LList<T> *pList, T item )
{
	for (LList<T> *pIt = pList; pIt != NULL; pIt = (LList<T> *)pIt->next) {
		if (pIt->data == item) {
			return pIt;
		}
	}
	return NULL;
}

template<class T>
LList<T>* LList<T>::remove( LList<T> *pList, T item )
{
	if (pList->data == item) {
		LList<T> *pTemp = (LList<T> *)pList->next;
		delete pList;
		pList = NULL;
		return pTemp;
	}
	LList<T> *pPrev = pList;
	for (LList<T> *pIt = (LList<T> *)pList->next; pIt != NULL; pIt = (LList<T> *)pIt->next) {
		if (pIt->data == item) {
			pPrev->next = pIt->next;
			delete pIt;
			pIt = NULL;
			break;
		}
		pPrev = pIt;
	}
	return pList;
}

template<class T>
LList<T>* LList<T>::push( LList<T> *pList, LList<T> *pItem )
{
	pItem->next = pList;
	return pItem;
}
