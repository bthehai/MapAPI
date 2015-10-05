#pragma once
#include "..\utils\pool\Inlist.h"
#include "..\utils\buffer\Buffer.h"

#ifndef MB
#define MB 1024 * 1024
#endif

#ifndef LIMIT_BUFFERS 
#define LIMIT_BUFFERS 16 * MB
#endif

class BufferObject : public Inlist {

private:
	int id;
	int size;
	int target;

private:
	static int m_nBufferMemoryUsage;
	static BufferObject* pool[2];
	static int counter[2];

private:
	BufferObject(int target, int id);

public:	
	virtual ~BufferObject();

public:
	void loadBufferData(GLvoid* &pBuffer, int newSize);
	void bind();
	void unbind();

public:
	static void checkBufferUsage(bool force);
	static BufferObject* get(int target, int size);
	static BufferObject* release(BufferObject *pBo);
	static int limitUsage(int reduce);
	static void createBuffers(int target, int num);
	static void clear();
	static void init(int num);
	static bool isMaxFill();

};