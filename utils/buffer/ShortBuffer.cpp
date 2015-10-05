#include "stdafx.h"
#include "ShortBuffer.h"
#include "ShortArrayBuffer.h"

ShortBuffer* ShortBuffer::allocate(int capacity) {
	if (capacity < 0) {
		throw std::exception("Capacitiy < 0");		
	}
	return (ShortBuffer *) new ShortArrayBuffer(new short[capacity], capacity);
}

ShortBuffer* ShortBuffer::wrap(short* arr, int length) {
	return wrap(arr, 0, length);
}

ShortBuffer* ShortBuffer::wrap(short* arr, int start, int shortCount) {
	ShortBuffer *pBuffer = new ShortArrayBuffer(arr, shortCount);
	pBuffer->position = start;
	pBuffer->limit = start + shortCount;
	return pBuffer;
}

ShortBuffer::ShortBuffer(int capacity, long effectiveDirectAddress) 
	: Buffer(1, capacity, effectiveDirectAddress)
{
}

ShortBuffer::~ShortBuffer() {

}

void* ShortBuffer::toArray()
{
	return protectedArray();
}

bool ShortBuffer::hasArray() {
	return protectedHasArray();
}

int ShortBuffer::arrayOffset()
{
	return protectedArrayOffset();
}

int ShortBuffer::compareTo( ShortBuffer* otherBuffer )
{
	int compareRemaining = (remaining() < otherBuffer->remaining()) ? remaining()
		: otherBuffer->remaining();
	int thisPos = position;
	int otherPos = otherBuffer->position;
	short thisByte, otherByte;
	while (compareRemaining > 0) {
		thisByte = get(thisPos);
		otherByte = otherBuffer->get(otherPos);
		if (thisByte != otherByte) {
			return thisByte < otherByte ? -1 : 1;
		}
		thisPos++;
		otherPos++;
		compareRemaining--;
	}
	return remaining() - otherBuffer->remaining();
}

bool ShortBuffer::isEquals(ShortBuffer* other )
{		
	if (remaining() != other->remaining()) {
		return false;
	}
	int myPosition = position;
	int otherPosition = other->position;
	bool equalSoFar = true;
	while (equalSoFar && (myPosition < limit)) {
		equalSoFar = get(myPosition++) == other->get(otherPosition++);
	}
	return equalSoFar;
}

ShortBuffer* ShortBuffer::get( short *dst, int length )
{
	return get(dst, 0, length);
}

ShortBuffer* ShortBuffer::get( short *dst, int offset, int shortCount )
{
	if (shortCount > remaining()) {
		throw std::exception("Buffer Under flow");
	}
	for (int i = offset; i < offset + shortCount; ++i) {
		dst[i] = get();
	}
	return this;
}

int ShortBuffer::hashCode()
{
	int myPosition = position;
	int hash = 0;
	while (myPosition < limit) {
		hash += get(myPosition++);
	}
	return hash;
}

ShortBuffer* ShortBuffer::put( short *src, int length )
{
	return put(src, 0, length);
}

ShortBuffer* ShortBuffer::put( short *src, int offset, int length )
{
	if (length > remaining()) {
		throw std::exception("Buffer Over flow");
	}
	for (int i = offset; i < offset + length; i++) {
		put(src[i]);
	}
	return this;
}

ShortBuffer* ShortBuffer::put( ShortBuffer *pSrc )
{
	if (isReadOnly()) {
		throw std::exception("Read Only Buffer");
	}
	if (pSrc == this) {
		throw std::exception("src == this");
	}
	if (pSrc->remaining() > remaining()) {
		throw std::exception("Buffer Over flow");
	}
	int len = pSrc->remaining();
	short *contents = new short[len];
	pSrc->get(contents, len);
	put(contents, len);

	return this;
}



