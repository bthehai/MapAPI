#pragma once
#include "..\utils\pool\Inlist.h"
#include "..\utils\buffer\MemoryBuffer.h"
#include "..\utils\pool\PoolX.h"

class BufferItem : public Inlist {
public:
	ByteMBuffer* byteBuffer;
	ShortMBuffer* sBuffer;
	FloatMBuffer* fBuffer;
	IntMBuffer* iBuffer;

	int size;

	void growBuffer(int size) {
		if (size < (1 << 15))
			size = (1 << 15);
		this->size = size;
		byte order = 1;
		byteBuffer = new ByteMBuffer(size);
		*byteBuffer += ByteMBuffer(&order, sizeof(byte));

		
		sBuffer = NULL;
		iBuffer = NULL;
		fBuffer = NULL;
	}
};

class NativeBufferPool : public PoolX {
private:
	BufferItem* m_pUsedBuffers;

protected:
	virtual BufferItem* createItem();
	virtual bool clearItem(Inlist* pItem);

public:
	BufferItem* get(int size);

	void releaseBuffers();

	ShortMBuffer* getShortBuffer(int size);
	FloatMBuffer* getFloatBuffer(int size);
	IntMBuffer* getIntBuffer(int size);

};