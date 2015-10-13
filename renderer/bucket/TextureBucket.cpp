#include "stdafx.h"
#include "TextureBucket.h"
#include "..\MapRenderer.h"
#include "..\GLState.h"
#include "..\GLViewport.h"

TextureItem::TexturePool * TextureBucket::pPool 
	= new TextureItem::TexturePool(POOL_FILL, TEXTURE_WIDTH, TEXTURE_HEIGHT);

TextureBucket::Shader * TextureBucket::pShader = NULL;

TextureBucket::TextureBucket( int type )
	: RenderBucket(type, false, true)
{
	pTextures = NULL;
	fixed = false;
}

TextureBucket::~TextureBucket()
{
	if (pTextures != NULL) {
		delete pTextures;
		pTextures = NULL;
	}
}

void TextureBucket::compile( ShortBuffer* vboData, ShortBuffer* iboData )
{
	for (TextureItem *pItem = pTextures; pItem != NULL; pItem = (TextureItem *)pItem->next) {
		pItem->upload();
	}
	compileVertexItems(vboData);
}

void TextureBucket::clear()
{
	TextureItem *pTemp;
	while (pTextures != NULL) {
		pTemp = pTextures;
		pTextures = pTextures->dispose();
		delete pTemp;
		pTemp = NULL;
	}
	RenderBucket::clear();
}

TextureItem* TextureBucket::getTextures()
{
	return pTextures;
}

void TextureBucket::render( int offset, int numIndices )
{
	glVertexAttribPointer(pShader->aPos, 4, GL_SHORT, false, 12, (void *)offset);

	glVertexAttribPointer(pShader->aTexCoord, 2, GL_SHORT, false, 12, (void *)(offset + 8));

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
}

TextureBucket::Shader::Shader()
{
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

bool TextureBucket::Shader::useProgram()
{
	if (GLShader::useProgram()) {
		GLState::enableVertexArrays(aPos, aTexCoord);
		return true;
	}
	return false;
}

TextureBucket::Shader::~Shader()
{

}

void TextureBucket::Renderer::init()
{
	pShader = new TextureBucket::Shader();
	pPool->init(0);
}

RenderBucket* TextureBucket::Renderer::draw( RenderBucket *pRBucket, GLViewport *pViewport, float scale )
{
	GLState::test(false, false);
	GLState::blend(true);

	pShader->useProgram();

	TextureBucket *pTB = (TextureBucket *)pRBucket;
	glUniform1f(pShader->uScale, pTB->fixed ? 1 / scale : 1);

	pViewport->proj.setAsUniform(pShader->uProj);
	pViewport->mvp.setAsUniform(pShader->uMV);

	MapRenderer::bindQuadIndicesVBO();

	//throw std::exception("The method or operation is not implemented.");

	for (TextureItem *pItem = pTB->pTextures; pItem != NULL; pItem = (TextureItem *)pItem->next) {
		glUniform2f(pShader->uTexSize, 
						1.0f / (pItem->width * COORD_SCALE),
						1.0f / (pItem->height * COORD_SCALE));
		pItem->bind();

		for (int i = 0; i < pItem->indices; i += MAX_INDICES) {

			//int off = (t.offset + i) * 8 + tb.vertexOffset;
			int off = (pItem->offset + i) * 8 + pTB->vertexOffset;
			
			int numIndices = pItem->indices - i;
			if (numIndices > MAX_INDICES)
				numIndices = MAX_INDICES;

			pTB->render(off, numIndices);
		}
	}

	return (RenderBucket *)pRBucket->next;
}

