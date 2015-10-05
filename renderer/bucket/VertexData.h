#pragma once

#include "..\..\utils\pool\SyncPool.h"
#include "..\..\utils\pool\Inlist.h"
#include "..\..\utils\buffer\ShortBuffer.h"

#define VERT_SIZE 360
#define MAX_POOL 500

class Chunk : public Inlist {
public:
	short vertices[VERT_SIZE];
	int used;
};

class Pool : public SyncPool {
public:
	Pool();
	virtual ~Pool();

protected:
	virtual Inlist* createItem();
	virtual bool clearItem(Inlist *pItem);

};

class VertexData : public List {

public:
	static Pool *pPool;

private:
	Chunk *pCur;
	int used;
	short *vertices;

public:
	VertexData();
	virtual ~VertexData();

public:
	int countSize();
	void dispose();
	int compile(ShortBuffer *pBuffer);

public:
	virtual Inlist* clear();

private:
	void getNext();

public:
	void add(short a);
	void add(short a, short b);
	void add(short a, short b, short c);
	void add(short a, short b, short c, short d);
	void add(short a, short b, short c, short d, short e, short f);

	void add(float a, float b);
	void add(float a, float b, float c);
	void add(float a, float b, float c, float d);
	void add(float a, float b, float c, float d, float e, float f);

	Chunk* obtainChunk();

	void releaseChunk();
	void releaseChunk(int size);

	void seek(int offset);

	bool empty();

public:
	static short toShort(float v);

};