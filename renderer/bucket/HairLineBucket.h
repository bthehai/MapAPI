#pragma once
#include "RenderBucket.h"

class GLViewport;

class HairLineBucket : public RenderBucket {

public:
	static class Renderer {
	public:
		static RenderBucket* draw(RenderBucket *pL, GLViewport *pV);
	};

};