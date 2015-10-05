#pragma once
#include "TreeNode.h"

template<class E>
class TileIndex {
	
protected:
	TreeNode<E> *pRoot;
	TreeNode<E> *pPool;

public:
	TileIndex();
	virtual ~TileIndex();

protected:
	void init();

public:
	static void checkIndex(int x, int y, int max);

public:
	virtual TreeNode<E>* create() = 0;
	virtual void removeItem(E item) = 0;

public:
	TreeNode<E>* add(int x, int y, int z);

	E getTile(int x, int y, int z);

	bool remove(TreeNode<E>* pItem);

	int size();

	void drop();

};

template<class E>
TileIndex<E>::TileIndex()
{
	pPool = NULL;
	pRoot = NULL;
}

template<class E>
void TileIndex<E>::init() {
	pRoot = create();
	pRoot->id = -1;
	pRoot->pParent = pRoot;
}


template<class E>
TileIndex<E>::~TileIndex()
{
	if (pRoot != NULL) {
		delete pRoot;
		pRoot = NULL;
	}
	if (pPool != NULL) {
		delete pRoot;
		pPool = NULL;
	}
}

template<class E>
void TileIndex<E>::checkIndex( int x, int y, int max )
{
	if (x < 0 || x >= max || y < 0 || y >= max) {
		char error[1024];
		sprintf_s(error, "invalid position %d/%d/%d", x, y, (max >> 1));
		throw std::exception(error);
	}
}


template<class E>
TreeNode<E>* TileIndex<E>::add( int x, int y, int z )
{
	checkIndex(x, y, 1 << z);
	if (z == 0) {
		return pRoot;
	}

	TreeNode<E>* pLeaf = pRoot;
	for (int level = z - 1; level >= 0; level--) {
		int id = ((x >> level) & 1) | ((y >> level) & 1) << 1;
		pLeaf->refs++;
		TreeNode<E> *pCur = NULL;
		switch (id) {
		case 0:
			pCur = pLeaf->pChild00;
			break;
		case 1:
			pCur = pLeaf->pChild01;
			break;
		case 2:
			pCur = pLeaf->pChild10;
			break;
		case 3:
			pCur = pLeaf->pChild11;
			break;
		}

		if (pCur != NULL) {
			pLeaf = pCur;
			continue;
		}

		if (pPool != NULL) {
			pCur = pPool;
			pPool = pPool->pParent;
		} else {
			pCur = create();
		}

		pCur->refs = 0;
		pCur->id = id;
		pCur->pParent = pLeaf;

		switch (id) {
		case 0:
			pCur->pParent->pChild00 = pCur;
			break;
		case 1:
			pCur->pParent->pChild01 = pCur;
			break;
		case 2:
			pCur->pParent->pChild10 = pCur;
			break;
		case 3:
			pCur->pParent->pChild11 = pCur;
			break;
		}

		pLeaf = pCur;
	}

	pLeaf->refs++;

	return pLeaf;
}

template<class E>
E TileIndex<E>::getTile( int x, int y, int z )
{
	checkIndex(x, y, 1 << z);

	if (z == 0)
		return pRoot->item;

	TreeNode<E> *pLeaf = pRoot;
	for (int level = z - 1; level >= 0; level--) {

		int id = ((x >> level) & 1) | ((y >> level) & 1) << 1;

		switch (id) {
		case 0:
			pLeaf = pLeaf->pChild00;
			break;
		case 1:
			pLeaf = pLeaf->pChild01;
			break;
		case 2:
			pLeaf = pLeaf->pChild10;
			break;
		case 3:
			pLeaf = pLeaf->pChild11;
			break;
		}

		if (pLeaf == NULL)
			return NULL;

		if (level == 0) {
			return pLeaf->item;
		}
	}
	return NULL;
}

template<class E>
bool TileIndex<E>::remove( TreeNode<E> *pItem )
{
	TreeNode<E> *pCur = pItem;

	while (pCur != pRoot) {
		if (pCur == NULL) {
			throw std::exception("Item not in index");
		}

		/* keep pointer to parent */
		TreeNode<E> *pNext = pCur->pParent;
		pCur->refs--;

		/* if current node has no children */
		if (pCur->refs == 0) {
			/* unhook from parent */
			switch (pCur->id) {
			case 0:
				delete pNext->pChild00;
				pNext->pChild00 = NULL;
				break;
			case 1:
				delete pNext->pChild01;
				pNext->pChild01 = NULL;
				break;
			case 2:
				delete pNext->pChild10;
				pNext->pChild10 = NULL;
				break;
			case 3:
				delete pNext->pChild11;
				pNext->pChild11 = NULL;
				break;
			}

			/* add item back to pool */
			pCur->pParent = pPool;
			pPool = pCur;
		}
		pCur = pNext;
	}

	pRoot->refs--;

	return true;
}

template<class E>
int TileIndex<E>::size()
{
	return pRoot->refs;
}

template<class E>
void TileIndex<E>::drop()
{
	Delete(pRoot->item);
	Delete(pRoot->pChild00);
	Delete(pRoot->pChild01);
	Delete(pRoot->pChild10);
	Delete(pRoot->pChild11);
}

