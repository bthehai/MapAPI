#pragma once

#include "ShortBuffer.h"
#include "ByteOrder.h"

class ShortArrayBuffer : public ShortBuffer {

public:
	ShortArrayBuffer(short* arr, int length);
	ShortArrayBuffer(int capacity, short* backingArray, int arrayOffset, bool isReadOnly);
	virtual ~ShortArrayBuffer();

private:
	short *m_backingArray;
	int m_nArrayOffset;
	bool m_bIsReadOnly;

private:
	static ShortArrayBuffer* copy(ShortArrayBuffer* pOther, int markOfOther, bool isReadOnly);

public:
	virtual ShortBuffer* asReadOnlyBuffer();
	virtual ShortBuffer* compact();
	virtual ShortBuffer* duplicate();
	virtual ShortBuffer* slice();
	
 	virtual ShortBuffer* put(short s);
 	virtual ShortBuffer* put(int index, short s);
	virtual ShortBuffer* put(short *src, int offset, int length);
	
	virtual bool isReadOnly();

	virtual int protectedArrayOffset();
	virtual short* protectedArray();	
	virtual bool protectedHasArray();

 	virtual short get();
 	virtual short get(int index);
	virtual ShortBuffer* get(short *dst, int offset, int shortCount);


	virtual bool isDirect();
	virtual ByteOrder* order();	

};