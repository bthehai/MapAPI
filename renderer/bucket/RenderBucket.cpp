#include "stdafx.h"
#include "RenderBucket.h"

VertexData * RenderBucket::EMPTY = new VertexData();


RenderBucket::RenderBucket( int type, bool indexed, bool quads )
{
	this->type = type;
	this->quads = quads;
	pVertexItems = new VertexData();
	if (indexed) {
		pIndiceItems = new VertexData();
	} else {
		pIndiceItems = EMPTY;
	}

	vertexOffset = 0;
	indiceOffset = 0;
	
}

RenderBucket::~RenderBucket()
{

}

void RenderBucket::clear()
{
	pVertexItems->dispose();
	pIndiceItems->dispose();
	numVertices = 0;
	numIndices = 0;
}

void RenderBucket::prepare()
{

}

int RenderBucket::getVertexOffset()
{
	return vertexOffset;
}

int RenderBucket::getIndiceOffset()
{
	return indiceOffset;
}

void RenderBucket::setVertexOffset( int offset )
{
	vertexOffset = offset;
}

void RenderBucket::compile(ShortBuffer* vboData, ShortBuffer* iboData) {
	compileVertexItems(vboData);
	if (iboData != NULL) {
		compileIndiceItems(iboData);
	}
}

void RenderBucket::compileVertexItems(ShortBuffer* vboData) {
	vertexOffset = vboData->getPosition() * 2; // FIXME 2? - should be vertex stride / num shorts
	pVertexItems->compile(vboData);
}

void RenderBucket::compileIndiceItems(ShortBuffer* iboData) {
	if (pIndiceItems == NULL || pIndiceItems->empty())
		return;

	indiceOffset = iboData->getPosition() * 2; // needs byte offset...
	pIndiceItems->compile(iboData);
}