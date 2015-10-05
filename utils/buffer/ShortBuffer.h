#pragma once
#include "Buffer.h"
#include "..\..\Basic.h"
#include "ByteOrder.h"

class ShortBuffer : public Buffer {

public:
	static ShortBuffer* allocate(int capacity);
	static ShortBuffer* wrap(short* arr, int length);
	static ShortBuffer* wrap(short* arr, int start, int shortCount);

protected:
	ShortBuffer(int capacity, long effectiveDirectAddress);

public:
	virtual ~ShortBuffer();

public:
	virtual void* toArray();
	virtual bool hasArray();
	virtual int arrayOffset();	

	virtual ShortBuffer* asReadOnlyBuffer() = 0;
	virtual ShortBuffer* compact() = 0;
	virtual ShortBuffer* duplicate() = 0;

	virtual bool isDirect() = 0;
	virtual ByteOrder* order() = 0;
	virtual short* protectedArray() = 0;
	virtual int protectedArrayOffset() = 0;
	virtual bool protectedHasArray() = 0;
	virtual ShortBuffer* put(short s) = 0;
	virtual ShortBuffer* put(int index, short s) = 0;
	virtual ShortBuffer* put(short *src, int offset, int length);
	virtual ShortBuffer* slice() = 0;
	
	virtual short get() = 0;
	virtual short get(int index) = 0;
	virtual ShortBuffer* get(short *dst, int offset, int shortCount);

	int compareTo(ShortBuffer* otherBuffer);

public:
	bool isEquals(ShortBuffer* other);
	
	int hashCode();
	ShortBuffer* get(short *dst, int length);
	
	ShortBuffer* put(short *src, int length);	
	ShortBuffer* put(ShortBuffer *pSrc);


};