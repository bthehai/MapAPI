#pragma once

template<class E>
class TreeNode {
public:
	TreeNode* pParent;
	TreeNode* pChild00;
	TreeNode* pChild01;
	TreeNode* pChild10;
	TreeNode* pChild11;

	E item;

	int id;
	int refs;

public:
	TreeNode() {
		pParent = NULL;
		pChild00 = NULL;
		pChild01 = NULL;
		pChild10 = NULL;
		pChild11 = NULL;
		item = (E) NULL;
	}

	virtual ~TreeNode() {	

		if (pChild00 != NULL) {
			delete pChild00;
			pChild00 = NULL;
		}

		if (pChild01 != NULL) {
			delete pChild01;
			pChild01 = NULL;
		}

		if (pChild10 != NULL) {
			delete pChild10;
			pChild10 = NULL;
		}

		if (pChild11 != NULL) {
			delete pChild11;
			pChild11 = NULL;
		}
	}

public:
	E parent() {
		return pParent->item;
	}

	E child(int i) {
		switch (i) {
			case 0: return (pChild00 != NULL) ? pChild00->item : NULL;
			case 1: return (pChild01 != NULL) ? pChild01->item : NULL;
			case 2: return (pChild10 != NULL) ? pChild10->item : NULL;
			case 3: return (pChild11 != NULL) ? pChild11->item : NULL;
		}
		return NULL;
	}

	bool isRoot() {
		return this == pParent;
	}
};