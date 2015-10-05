#include "stdafx.h"
#include "ShortArrayBuffer.h"


ShortArrayBuffer::ShortArrayBuffer( short* arr, int length )
	//: ShortArrayBuffer(length, arr, 0, false)
	: ShortBuffer(length, 0)
{
	this->m_backingArray = arr;
	this->m_nArrayOffset = 0;
	this->m_bIsReadOnly = false;
}

ShortArrayBuffer::ShortArrayBuffer(int capacity, short* backingArray, int arrayOffset, bool isReadOnly) 
	: ShortBuffer(capacity, 0)
{	
	this->m_backingArray = backingArray;
	this->m_nArrayOffset = arrayOffset;
	this->m_bIsReadOnly = isReadOnly;
}

ShortArrayBuffer::~ShortArrayBuffer() 
{
	if (m_backingArray != NULL) {
		delete []m_backingArray;
		m_backingArray = NULL;
	}
}

ShortArrayBuffer* ShortArrayBuffer::copy(ShortArrayBuffer* pOther, int markOfOther, bool isReadOnly) {
	ShortArrayBuffer* pBuffer = new ShortArrayBuffer(
			pOther->getCapacity(), 
			pOther->m_backingArray, 
			pOther->m_nArrayOffset, 
			isReadOnly);
	pBuffer->limit = pOther->limit;
	pBuffer->position = pOther->getPosition();
	pBuffer->mark = markOfOther;
	return pBuffer;
}

ShortBuffer* ShortArrayBuffer::asReadOnlyBuffer()
{
	return copy(this, mark, true);
}

ShortBuffer* ShortArrayBuffer::compact()
{
	if (m_bIsReadOnly) {
		throw std::exception("Read only buffer");
	}
	//memcpy(m_backingArray + arrayOffset, m_backingArray + (arrayOffset + position), remaining());
	position = limit - position;
	limit = capacity;
	mark = UNSET_MARK;
	return this;
}

ShortBuffer* ShortArrayBuffer::duplicate()
{
	return (ShortBuffer *)ShortArrayBuffer::copy(this, mark, isReadOnly());
}

ShortBuffer* ShortArrayBuffer::slice()
{
	return (ShortBuffer *)new ShortArrayBuffer(remaining(), m_backingArray, m_nArrayOffset + position, isReadOnly());
}

bool ShortArrayBuffer::isReadOnly()
{
	return m_bIsReadOnly;
}

short* ShortArrayBuffer::protectedArray()
{
	if (isReadOnly()) {
		throw std::exception("Read Only Buffer");
	}
	return m_backingArray;
}

int ShortArrayBuffer::protectedArrayOffset()
{
	if (isReadOnly()) {
		throw std::exception("Read Only Buffer");
	}
	return m_nArrayOffset;
}

bool ShortArrayBuffer::protectedHasArray()
{
	if (isReadOnly()) {
		return false;
	}
	return true;
}

short ShortArrayBuffer::get()
{
	if (position == limit) {
		throw std::exception("Buffer Under Flow");
	}
	return m_backingArray[m_nArrayOffset + position++];
}

short ShortArrayBuffer::get( int index )
{
	checkIndex(index);
	return m_backingArray[m_nArrayOffset + index];
}

ShortBuffer* ShortArrayBuffer::get( short *dst, int offset, int shortCount )
{
	if (shortCount > remaining()) {
		throw std::exception("Buffer Under flow");
	}
	//memcpy(m_backingArray + (arrayOffset + position), dst + offset, shortCount);
	memcpy(dst + offset, m_backingArray + (m_nArrayOffset + position), shortCount);
	position += shortCount;
	return this;
}

bool ShortArrayBuffer::isDirect()
{
	return false;
}

ByteOrder* ShortArrayBuffer::order()
{
	return ByteOrder::nativeOrder();
}

ShortBuffer* ShortArrayBuffer::put( short s )
{
	if (isReadOnly()) {
		throw std::exception("Read only Buffer");
	}
	if (position == limit) {
		throw std::exception("Buffer over flow");
	}
	m_backingArray[m_nArrayOffset + position++] = s;
	return this;
}

ShortBuffer* ShortArrayBuffer::put( int index, short s )
{
	if (isReadOnly()) {
		throw std::exception("Read only Buffer");
	}	
	checkIndex(index);
	m_backingArray[m_nArrayOffset + index] = s;
	return this;
}

ShortBuffer* ShortArrayBuffer::put( short *src, int offset, int length )
{
	if (isReadOnly()) {
		throw std::exception("Read only Buffer");
	}	
	if (length > remaining()) {
		throw std::exception("Buffer over flow");
	}
	memcpy(&m_backingArray + (m_nArrayOffset + position), src + offset, length);
	position += length;
	return this;
}