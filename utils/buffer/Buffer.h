#pragma once

#define UNSET_MARK -1

class Buffer {
public:
	const int capacity;
	const int _elementSizeShift;
	const long effectiveDirectAddress;

	int limit;
	int mark; /* = UNSET_MARK*/
	int position;/* = 0*/


protected:
	Buffer(int elementSizeShift, int capacity, long effectiveDirectAddress);

public:
	virtual ~Buffer();


public:
	virtual void* toArray() = 0;
	virtual int arrayOffset() = 0;
	virtual bool hasArray() = 0;
	virtual bool isDirect() = 0;
	virtual bool isReadOnly() = 0;

public:
	int getCapacity() {
		return capacity;
	}

	int getLimit() {
		return limit;
	}

	int getPosition() {
		return position;
	}

	bool hasRemaining() {
		return position < limit;
	}

	void checkIndex(int index);
	void checkIndex(int index, int sizeOfType);
	void checkWritable();
	int checkGetBounds(int bytesPerElement, int length, int offset, int count);
	int checkPutBounds(int bytesPerElement, int length, int offset, int count);
	void checkStartEndRemaining(int start, int end);

	void positionImpl(int newPosition);
	int remaining() {
		return limit - position;
	}

	int getElementSizeShift() {
		return _elementSizeShift;
	}
	
public:
	Buffer* clear();
	Buffer* flip();
	Buffer* setLimit(int newLimit);
	Buffer* setMark();
	Buffer* setPosition(int newPosition);
	Buffer* reset();
	Buffer* rewind();

};