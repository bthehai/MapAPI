#pragma once
#include "RenderBucket.h"

class GLViewport;
class GLMatrix;
class RenderBuckets;

class LineBucket : public RenderBucket {

protected:
	virtual void prepare();

public:
	static class Renderer {
	public:
		static RenderBucket* draw(RenderBucket *pBucket, GLViewport *pViewport, float scale, RenderBuckets *pBuckets);
	};
};