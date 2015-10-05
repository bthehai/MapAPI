#pragma once

#include "RenderBucket.h"
#include "TextureItem.h"
#include "..\GLShader.h"
#include "..\GLState.h"
#include "..\GLViewport.h"

#define INDICES_PER_SPRITE 6
#define VERTICES_PER_SPRITE 4
#define SHORTS_PER_VERTICE 6

#define TEXTURE_HEIGHT 256
#define TEXTURE_WIDTH 1024

#define POOL_FILL 4

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
	static class Shader : public GLShader {
	public:
		int uMV, uProj, uScale, uTexSize, aPos, aTexCoord;
		
	public:
		Shader() {
			if (!create("texture_layer")) {
				return;
			}
			uMV = getUniform("u_mv");
			uProj = getUniform("u_proj");
			uScale = getUniform("u_scale");
			uTexSize = getUniform("u_div");
			aPos = getAttrib("vertex");
			aTexCoord = getAttrib("tex_coord");
		}

		virtual bool useProgram() {
			if (GLShader::useProgram()) {
				GLState::enableVertexArrays(aPos, aTexCoord);
				return true;
			}
			return false;
		}

	};

	static Shader *pShader;

	static class Renderer {
	public:
		static void init() {
			pShader = new TextureBucket::Shader();
			pPool->init(0);
		}

		static RenderBucket* draw(RenderBucket *pRBucket, GLViewport *pViewport, float scale) {
			GLState::test(false, false);
			GLState::blend(true);

			pShader->useProgram();

			TextureBucket *pTB = (TextureBucket *)pRBucket;
			glUniform1f(pShader->uScale, pTB->fixed ? 1 / scale : 1);

			pViewport->proj.setAsUniform(pShader->uProj);
			pViewport->mvp.setAsUniform(pShader->uMV);

			//MapRenderer::bindQ
		}
	};

	
};