#pragma once
#include "..\..\layers\tile\MapTile.h"
#include "..\BufferObject.h"
#include "RenderBucket.h"
#include "LineBucket.h"
#include "HairLineBucket.h"
#include "PolygonBucket.h"
#include "MeshBucket.h"
#include "..\..\theme\styles\LineStyle.h"
#include "..\..\theme\styles\AreaStyle.h"


#define SHORT_BYTES sizeof(short)

#define VS_LINE_CNT			4
#define VS_TEXLINE_CNT		6
#define VS_POLY_CNT			2
#define VS_MESH_CNT			2
#define VS_EXTRUSION_CNT	4
#define VS_HAIRLINE_CNT		2
#define VS_SYMBOL_CNT		6
#define VS_BITMAP_CNT		6

class RenderBuckets : public TileData {

private:
	RenderBucket *m_pBuckets;
	RenderBucket *m_pCurBucket;

public:
	BufferObject *pVbo;
	BufferObject *pIbo;

	int offset[2];

public:
	RenderBuckets();
	virtual ~RenderBuckets();

protected:
	virtual void dispose();

public:
	LineBucket* addLineBucket(int level, LineStyle *pStyle);
	HairLineBucket* addHairLineBucket(int level, LineStyle *pStyle);
	PolygonBucket* addPolygonBucket(int level, AreaStyle *pStyle);
	MeshBucket* addMeshBucket(int level, AreaStyle *pStyle);
	

	LineBucket* getLineBucket(int level);
	HairLineBucket* getHairLineBucket(int level);
	PolygonBucket* getPolygonBucket(int level);
	MeshBucket* getMeshBucket(int lelve);

public:
	void set(RenderBucket *pBucket);
	RenderBucket* get();

	void setFrom(RenderBuckets *pBuckets);
	void clear();
	void clearBuckets();
	void prepare();
	void bind();

	bool compile(bool addFill);

private:
	RenderBucket* getBucket(int level, int type);
	int countVboSize();
	int countIboSize();

	static short s;
	static short fillCoords[8];

public:
	static void initRenderer();

};