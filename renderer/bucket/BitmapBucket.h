#pragma once
#include "TextureBucket.h"

class GLViewport;

class BitmapBucket : public TextureBucket {

public:
	static class Renderer {
	public:
		static void init();
		static RenderBucket* draw(RenderBucket* pB, GLViewport *pV, float scale, float alpha);
	};
};