#pragma once

#include "RenderBucket.h"
#include "TextureItem.h"
#include "..\GLShader.h"


#define INDICES_PER_SPRITE 6
#define VERTICES_PER_SPRITE 4
#define SHORTS_PER_VERTICE 6

#define TEXTURE_HEIGHT 256
#define TEXTURE_WIDTH 1024

#define POOL_FILL 4

class GLViewport;


class TextureBucket : public RenderBucket {

public:
	static TextureItem::TexturePool *pPool;

public:
	TextureBucket(int type);
	virtual ~TextureBucket();

public:
	TextureItem *pTextures;
	bool fixed;

protected:
	virtual void compile(ShortBuffer* vboData, ShortBuffer* iboData);
	virtual void clear();

public:
	TextureItem* getTextures();
	void render(int offset, int numIndices);

public:
	static class Shader : public GLShader {
	public:
		int uMV, uProj, uScale, uTexSize, aPos, aTexCoord;
		
	public:
		Shader();
		virtual ~Shader();

	public:
		virtual bool useProgram();
	};

	static Shader *pShader;

	static class Renderer {
	public:
		static void init();

		static RenderBucket* draw(RenderBucket *pRBucket, GLViewport *pViewport, float scale); 

	};
};