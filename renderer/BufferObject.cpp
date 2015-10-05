#include "stdafx.h"
#include "BufferObject.h"
#include "GLState.h"
#include "GLUtils.h"
#include "..\backend\GLAdapter.h"


int BufferObject::m_nBufferMemoryUsage = 0;
int BufferObject::counter[2];
BufferObject* BufferObject::pool[2];

BufferObject::BufferObject(int target, int id)
{
	this->id = id;
	this->target = target;
}

BufferObject::~BufferObject()
{

}

void BufferObject::loadBufferData( GLvoid* &pBuffer, int newSize )
{
	bool clear = false;
// 	if (pBuffer->getPosition() != 0) {
// 		pBuffer->flip();
// 	}

	GLState::bindBuffer(target, id);

	if (!GLAdapter::NO_BUFFER_SUB_DATA && !clear && (size > newSize) && (size < newSize * 4)) {
		glBufferSubData(target, 0, newSize, pBuffer);
	} else {
		m_nBufferMemoryUsage += newSize - size;
		size = newSize;
		glBufferData(target, size, pBuffer, GL_STATIC_DRAW);
	}

}

void BufferObject::bind()
{
	GLState::bindBuffer(target, id);
}

void BufferObject::unbind()
{
	GLState::bindBuffer(target, 0);
}

void BufferObject::checkBufferUsage( bool force )
{
	if (m_nBufferMemoryUsage < LIMIT_BUFFERS)
		return;
	
	m_nBufferMemoryUsage -= BufferObject::limitUsage(MB);

}

BufferObject* BufferObject::get( int target, int size )
{
	int t = (target == GL_ARRAY_BUFFER) ? 0 : 1;

	if (pool[t] == NULL) {
		if (counter[t] != 0)
			throw std::exception("lost objects");

		createBuffers(target, 10);
		counter[t] += 10;
	}
	counter[t]--;

	if (size != 0) {
		/* find the item with minimal size greater 'size' bytes. */
		BufferObject* pBo = pool[t];
		/* actually points to BufferObject before min */
		BufferObject* pMin = NULL;
		BufferObject* pPrev = NULL;

		for (; pBo != NULL; pBo = (BufferObject *)pBo->next) {
			if (pBo->size > size) {
				BufferObject *pTemp = (BufferObject *)pMin->next;
				if (pMin == NULL || pTemp->size > pBo->size)
					pMin = pPrev;
			}
			pPrev = pBo;
		}

		if (pMin != NULL && pMin != pool[t]) {
			pBo = (BufferObject *)pMin->next;
			pMin->next = pBo->next;
			pBo->next = NULL;
			return pBo;
		}
	}

	BufferObject* pBo = pool[t];
	pool[t] = (BufferObject *)pool[t]->next;
	pBo->next = NULL;
	return pBo;
}

BufferObject* BufferObject::release( BufferObject *pBo )
{
	if (pBo == NULL)
		return NULL;

	// if (counter > 200) {
	// log.debug("should clear some buffers " + counter);
	// }
	int t = (pBo->target == GL_ARRAY_BUFFER) ? 0 : 1;

	pBo->next = pool[t];
	pool[t] = pBo;
	counter[t]++;

	return NULL;
}

int BufferObject::limitUsage( int reduce )
{
	int vboIds[10];
	int freed = 0;

	for (int t = 0; t < 2; t++) {

		int removed = 0;
		BufferObject* pPrev = pool[t];

		if (pPrev == NULL) {
			//log.debug("nothing to free");
			continue;
		}

		for (BufferObject* pBo = (BufferObject *)pool[t]->next; pBo != NULL;) {
			if (pBo->size > 0) {
				freed += pBo->size;
				pBo->size = 0;

				vboIds[removed++] = pBo->id;
				pPrev->next = pBo->next;
				pBo = (BufferObject *)pBo->next;

				if (removed == 10 || reduce < freed)
					break;

			} else {
				pPrev = pBo;
				pBo = (BufferObject *)pBo->next;
			}
		}

		if (removed > 0) {
			GLUtils::glDeleteBuffers(removed, vboIds);
			counter[t] -= removed;
		}

	}

	return freed;
}

void BufferObject::createBuffers( int target, int num )
{
	GLuint* mVboIds = GLUtils::glGenBuffers_(num);

	int t = (target == GL_ARRAY_BUFFER) ? 0 : 1;

	for (int i = 0; i < num; i++) {
		BufferObject* pBo = new BufferObject(target, mVboIds[i]);
		pBo->next = pool[t];
		pool[t] = pBo;
	}

	if (mVboIds != NULL) {
		delete []mVboIds;
	}
}

void BufferObject::clear()
{
	m_nBufferMemoryUsage = 0;

	if (pool[0] != NULL) {
		delete pool[0];
		pool[0] = NULL;
	}
	if (pool[1] != NULL) {
		delete pool[1];
		pool[1] = NULL;
	}
	counter[0] = 0;
	counter[1] = 0;
}

void BufferObject::init(int num)
{
	createBuffers(GL_ARRAY_BUFFER, num);
	counter[0] += num;
}

bool BufferObject::isMaxFill()
{
	return m_nBufferMemoryUsage  > LIMIT_BUFFERS;
}



