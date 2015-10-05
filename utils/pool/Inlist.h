#pragma once
#include "..\..\Basic.h"



class Inlist {

public:
	Inlist* next;

public:
	static int size(Inlist *pList);
	static Inlist* push(Inlist *pList, Inlist *pItem);
	static Inlist* remove(Inlist *pList, Inlist *pItem);
	static Inlist* get(Inlist *pList, int i);
	static Inlist* appendItem(Inlist *pList, Inlist *pItem);
	static Inlist* appendList(Inlist *pList, Inlist *pOther);
	static Inlist* last(Inlist *pList);
	static Inlist* prependRelative(Inlist *pList, Inlist *pItem, Inlist *pOther);

};

class List : public Iterable<Inlist*>, Iterator<Inlist*> {
private:
	Inlist *pHead;
	Inlist *pCur;

public:
	List();
	virtual ~List();

public:
	void push(Inlist *pItem);
	Inlist* pop();
	void reverse();
	void append(Inlist *pItem);
	void appendList(Inlist *pList);
	void remove(Inlist *pItem);
	virtual Inlist* clear();
	Inlist* head();

	int size();

public:
	virtual bool hasNext();
	virtual Inlist* next();
	virtual void remove();

	virtual Iterator<Inlist*>* iterator();


};