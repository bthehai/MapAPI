#include "stdafx.h"
#include "TextureBucket.h"

TextureItem::TexturePool * TextureBucket::pPool 
	= new TextureItem::TexturePool(POOL_FILL, TEXTURE_WIDTH, TEXTURE_HEIGHT);

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



