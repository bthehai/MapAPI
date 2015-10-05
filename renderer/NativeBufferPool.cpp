#include "stdafx.h"
#include "NativeBufferPool.h"


BufferItem* NativeBufferPool::createItem()
{
	return NULL;
}

bool NativeBufferPool::clearItem( Inlist* pItem )
{
	return false;
}

BufferItem* NativeBufferPool::get( int size )
{
	BufferItem *pBI = (BufferItem *)m_pPool;
	if (pBI == NULL) {
		pBI = new BufferItem();
	} else {
		m_pPool = (BufferItem *)pBI->next;
		pBI->next = NULL;
	}
	if (pBI->size < size) {
		pBI->growBuffer(size);
	}
	m_pUsedBuffers = (BufferItem *)Inlist::push(m_pUsedBuffers, pBI);

	return pBI;
}

void NativeBufferPool::releaseBuffers()
{
	m_pUsedBuffers = (BufferItem *)releaseAll(m_pUsedBuffers);
}

ShortMBuffer* NativeBufferPool::getShortBuffer( int size )
{
	BufferItem *pItem = get(size * sizeof(short));
	if (pItem->sBuffer == NULL) {
		pItem->byteBuffer->empty();
		//pItem->sBuffer = pItem->byteBuffer->asIntBuffer();
	} else {
		pItem->sBuffer->empty();
	}
	return pItem->sBuffer;
}

FloatMBuffer* NativeBufferPool::getFloatBuffer( int size )
{
	BufferItem *pItem = get(size * sizeof(float));
	if (pItem->fBuffer == NULL) {
		pItem->byteBuffer->empty();
		//pItem->iBuffer = pItem->byteBuffer->asIntBuffer();
	} else {
		pItem->fBuffer->empty();
	}
	return pItem->fBuffer;
}

IntMBuffer* NativeBufferPool::getIntBuffer( int size )
{
	BufferItem *pItem = get(size * sizeof(int));
	if (pItem->iBuffer == NULL) {
		pItem->byteBuffer->empty();
		//pItem->iBuffer = pItem->byteBuffer->asIntBuffer();
	} else {
		pItem->iBuffer->empty();
	}
	return pItem->iBuffer;
}
