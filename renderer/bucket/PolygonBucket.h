#pragma once
#include "RenderBucket.h"

class GLViewport;
class GLMatrix;

class PolygonBucket : public RenderBucket {

public:
	enum {
		CLIP_STENCIL = 1,
		CLIP_DEPTH = 2,
		CLIP_TEST_DEPTH = 3,
	};

public:
	static class Renderer {
	public:
		static RenderBucket* draw(RenderBucket *pBuckets, GLViewport *pViewport, float div, bool first);
		static void clip(GLMatrix *pMVP, int clipMode);
		static void drawOver(GLMatrix *pMVP, int color, float alpha);
	};
};