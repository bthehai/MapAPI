#pragma once
#include "VertexData.h"
#include "..\..\utils\pool\Inlist.h"
#include "..\..\utils\buffer\ShortBuffer.h"


class RenderBucket : public Inlist {

public:
	enum TYPE_RENDER {
		LINE = 0,
		TEXLINE = 1,
		POLYGON = 2,
		MESH = 3,
		EXTRUSION = 4,
		HAIRLINE = 5,
		SYMBOL = 6,
		BITMAP = 7,
		SDF = 8,
	};

public:
	int type;
	int level;
	bool quads;

protected:
	VertexData* pVertexItems;
	VertexData* pIndiceItems;

	int numVertices;
	int numIndices;

	int vertexOffset;
	int indiceOffset;

public:
	static VertexData *EMPTY;

protected:
	RenderBucket(int type, bool indexed, bool quads);

public:
	virtual ~RenderBucket();

public:
	virtual void clear();
	virtual void compile(ShortBuffer* vboData, ShortBuffer* iboData);
	void compileVertexItems(ShortBuffer* vboData);
	void compileIndiceItems(ShortBuffer* iboData);

public:
	int getVertexOffset();
	int getIndiceOffset();
	void setVertexOffset(int offset);

protected:
	virtual void prepare();

};