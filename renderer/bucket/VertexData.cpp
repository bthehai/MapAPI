#include "stdafx.h"
#include "VertexData.h"
#include "..\..\utils\FastMath.h"

Pool::Pool() 
	: SyncPool(MAX_POOL)
{

}

Pool::~Pool() {
}

Inlist* Pool::createItem() {
	return (Inlist *)new Chunk();
}

bool Pool::clearItem( Inlist *pItem ) {
	((Chunk *)pItem)->used = 0;
	return true;
}

Pool* VertexData::pPool = new Pool();

VertexData::VertexData() {
	pCur = NULL;
	used = VERT_SIZE;
	vertices = NULL;
}

VertexData::~VertexData() {
}


int VertexData::countSize() {
	if (pCur == NULL)
		return 0;
	pCur->used = used;
	int size = 0;
	for (Inlist *pItr = head(); pItr != NULL; pItr = pItr->next) {
		size += ((Chunk *)pItr)->used;
	}
	return size;
}

void VertexData::dispose() {
	pPool->releaseAll(List::clear());
	used = VERT_SIZE;
	pCur = NULL;
	vertices = NULL;
}

Inlist* VertexData::clear() {
	if (pCur == NULL) {
		return (Inlist *)pCur;
	}

	pCur->used = used;
	used = VERT_SIZE;
	pCur = NULL;
	vertices = NULL;

	return List::clear();
}

int VertexData::compile(ShortBuffer *pBuffer) {
	if (pCur == NULL)
		return 0;
	pCur->used = used;
	int size = 0;

	for (Inlist *pIt = head(); pIt != NULL; pIt = pIt->next) {
		Chunk *pChunk = (Chunk *)pIt;
		size += pChunk->used;
		pBuffer->put(pChunk->vertices, 0, pChunk->used);
		//*pBuffer += MemoryBuffer<short>(pChunk->vertices, pChunk->used);		
	}
	dispose();
	return size;
}

void VertexData::getNext()
{
	if (pCur == NULL) {
		pCur = (Chunk *)pPool->get();
		push((Inlist *)pCur);
	} else {
		if (pCur->next != NULL) {
			throw std::exception("Seek...");
		}

		pCur->used = VERT_SIZE;
		pCur->next = (Chunk *)pPool->get();
		pCur = (Chunk *)pCur->next;
	}
	vertices = pCur->vertices;
	used = 0;
}

short VertexData::toShort( float v )
{
	return (short) clamp(v, 
		(float) (-2 << 15), (float) ((2 << 15) - 1));
}

void VertexData::add( short a )
{
	if (used == VERT_SIZE)
		getNext();

	vertices[used++] = a;
}

void VertexData::add( short a, short b )
{
	if (used == VERT_SIZE)
		getNext();

	vertices[used + 0] = a;
	vertices[used + 1] = b;
	used += 2;
}

void VertexData::add( short a, short b, short c )
{
	if (used == VERT_SIZE)
		getNext();

	vertices[used + 0] = a;
	vertices[used + 1] = b;
	vertices[used + 2] = c;
	used += 3;
}

void VertexData::add( short a, short b, short c, short d )
{
	if (used == VERT_SIZE)
		getNext();

	vertices[used + 0] = a;
	vertices[used + 1] = b;
	vertices[used + 2] = c;
	vertices[used + 3] = d;
	used += 4;
}

void VertexData::add( short a, short b, short c, short d, short e, short f )
{
	if (used == VERT_SIZE)
		getNext();

	vertices[used + 0] = a;
	vertices[used + 1] = b;
	vertices[used + 2] = c;
	vertices[used + 3] = d;
	vertices[used + 4] = e;
	vertices[used + 5] = f;
	used += 6;
}

void VertexData::add( float a, float b )
{
	add(toShort(a), toShort(b));
}

void VertexData::add( float a, float b, float c )
{
	add(toShort(a), toShort(b), toShort(c));
}

void VertexData::add( float a, float b, float c, float d )
{
	add(toShort(a), toShort(b), toShort(c), toShort(d));
}

void VertexData::add( float a, float b, float c, float d, float e, float f )
{
	add(toShort(a), toShort(b), toShort(c), toShort(d), toShort(e), toShort(f));
}

Chunk* VertexData::obtainChunk()
{
	if (used == VERT_SIZE) {
		getNext();
	}
	pCur->used = used;

	return pCur;
}

void VertexData::releaseChunk() {
	used = pCur->used;
}

void VertexData::releaseChunk(int size) {
	pCur->used = size;
	used = size;
}

void VertexData::seek( int offset )
{
	used += offset;
	pCur->used = used;
	if (used > VERT_SIZE || used < 0) {
		throw std::exception("Seek too far");
	}
}

bool VertexData::empty()
{
	return pCur == NULL;
}




