#pragma once
#include "..\map\Map.h"
#include "GLViewport.h"
#include "NativeBufferPool.h"

#ifndef COORD_SCALE
#define COORD_SCALE 8.0f
#endif

#define MAX_QUADS 512
#define MAX_INDICES (MAX_QUADS * 6)

class MapRenderer {

private:
	Map* m_pMap;
	GLViewport* m_pViewPort;

	bool m_bNewSurface;

	static float *m_ClearColor;

	static int m_nQuadIndicesID;
	static int m_nQuadVerticesID;

	static bool m_bRerender;
	static NativeBufferPool* m_pBufferPool;

public:
	static long frameTime;


public:
	MapRenderer(Map *pMap);
	virtual ~MapRenderer();

public:
	static void setBackgroundColor(int color);

public:
	void onDrawFrame();
	void onSurfaceChanged(int width, int height);
	void onSurfaceCreate();

private:
	void draw();

public:
	static void bindQuadVertexVBO(int location);
	static void bindQuadIndicesVBO();
	static void animate();

	static FloatMBuffer* getFloatBuffer(int size);
	static ShortMBuffer* getShortBuffer(int size);
	static IntMBuffer* getIntBuffer(int size);
};