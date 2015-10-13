#pragma once
#include "RenderBucket.h"

class GLViewport;
class RenderBuckets;

class LineTexBucket : public RenderBucket {

public:
	static class Renderer {
	public:
		static RenderBucket* draw(RenderBucket *pB, GLViewport *pV, float div, RenderBuckets *pBuckets);
	};

};