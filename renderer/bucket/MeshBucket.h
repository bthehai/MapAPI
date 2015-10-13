#pragma once
#include "RenderBucket.h"

class GLViewport;

class MeshBucket : public RenderBucket {

public:
	static class Renderer {
	public:
		static RenderBucket* draw(RenderBucket *pL, GLViewport *pV);
	};
};