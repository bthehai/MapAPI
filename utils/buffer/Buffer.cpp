#include "stdafx.h"
#include "Buffer.h"

// ess = elementSizeShift
// cap = capacity
// eda = effectiveDirectAddress
Buffer::Buffer(int ess, int cap, long eda) 
	: _elementSizeShift(ess)
	, capacity(cap)
	, effectiveDirectAddress(eda)
{
	limit = capacity;
	mark = UNSET_MARK;
	position = 0;
}

Buffer::~Buffer() {

}

void Buffer::checkIndex( int index )
{
	if (index < 0 || index >= limit)
		throw std::exception("Index Out of Bound");
}

void Buffer::checkIndex( int index, int sizeOfType )
{
	if (index < 0 || index > limit - sizeOfType) {
		throw std::exception("Index Out of Bound by sizeOfType");
	}	
}

void Buffer::checkWritable() {
	if (isReadOnly()) {
		throw std::exception("Read-only buffer");
	}
}

int Buffer::checkGetBounds( int bytesPerElement, int length, int offset, int count )
{
	int byteCount = bytesPerElement * count;
	if ((offset | count) < 0 || offset > length || length - offset < count) {
		throw std::exception("Index Out of Bound - CheckGetBounds");
	}
	if (byteCount > remaining()) {
		throw std::exception("Buffer Under flow");
	}
	return byteCount;
}

int Buffer::checkPutBounds( int bytesPerElement, int length, int offset, int count )
{
	int byteCount = bytesPerElement * count;
	if ((offset | count) < 0 || offset > length || length - offset < count) {
		throw std::exception("Index Out of Bound - CheckPutBounds");
	}
	if (byteCount > remaining()) {
		throw std::exception("Buffer Over flow");
	}
	if (isReadOnly()) {
		throw std::exception("Read Only Buffer");
	}
	return byteCount;
}

void Buffer::checkStartEndRemaining( int start, int end )
{
	if (end < start || start < 0 || end > remaining()) {
		throw std::exception("Index Out of Bound - checkStartEndRemaining");
	}
}

Buffer* Buffer::clear()
{
	position = 0;
	mark = UNSET_MARK;
	limit = capacity;
	return this;
}

Buffer* Buffer::flip()
{
	limit = position;
	position = 0;
	mark = UNSET_MARK;
	return this;
}

Buffer* Buffer::setLimit( int newLimit )
{
	if (newLimit < 0 || newLimit > capacity) {		
		throw std::exception("Bad limit");
	}
	limit = newLimit;
	if (position > newLimit) {
		position = newLimit;
	}
	if ((mark != UNSET_MARK) && (mark > newLimit)) {
		mark = UNSET_MARK;
	}
	return this;
}

Buffer* Buffer::setMark()
{
	mark = position;
	return this;
}

Buffer* Buffer::setPosition( int newPosition )
{
	positionImpl(newPosition);
	return this;
}

void Buffer::positionImpl( int newPosition )
{
	if (newPosition < 0 || newPosition > limit) {
		throw std::exception("Bad position");
	}
	position = newPosition;
	if ((mark != UNSET_MARK) && (mark > position)) {
		mark = UNSET_MARK;
	}
}

Buffer* Buffer::reset()
{
	if (mark == UNSET_MARK) {
		throw std::exception("Mark not set");
	}
	position = mark;
	return this;
}

Buffer* Buffer::rewind()
{
	position = 0;
	mark = UNSET_MARK;
	return this;
}

