#include "stdafx.h"
#include "TextureItem.h"
#include "..\..\backend\CanvasAdapter.h"
#include "..\..\backend\canvas\Color.h"
#include "..\GLUtils.h"
#include "..\GLState.h"

TextureItem::TexturePool::TexturePool( int maxFill, int width, int height ) 
	: SyncPool(maxFill)
{
	m_pBitmaps = new std::vector<Bitmap*>();
	m_nHeight = height;
	m_nWidth = width;
	m_bUseBitmapPool = true;
	m_nTexCnt = 0;
	
}

TextureItem::TexturePool::TexturePool(int maxFill) 
	: SyncPool(maxFill)
{
	m_nWidth = 0;
	m_nHeight = 0;
	m_bUseBitmapPool = false;
}

TextureItem::TexturePool::~TexturePool() {
}

Inlist* TextureItem::TexturePool::get()
{
	TextureItem *pItem = (TextureItem *)SyncPool::get();
	if (!m_bUseBitmapPool) {
		return pItem;
	}

	int size = m_pBitmaps->size();
	if (size == 0) {
		pItem->pBitmap = CanvasAdapter::newBitmap(m_nWidth, m_nHeight, 0);
	} else {
		pItem->pBitmap = m_pBitmaps->at(size - 1);
		pItem->pBitmap->eraseColor(Color::TRANSPARENT);
		m_pBitmaps->pop_back();	
	}
	return (Inlist *)pItem;
}

Inlist* TextureItem::TexturePool::get( Bitmap *pBmp )
{
	TextureItem *pItem = (TextureItem *)SyncPool::get();
	pItem->pBitmap = pBmp;
	return pItem;
}

Inlist* TextureItem::TexturePool::releaseAll( Inlist *pItem )
{
	throw std::exception("use TextureItem.dispose()");
}

Inlist* TextureItem::TexturePool::createItem()
{
	return (Inlist *) new TextureItem(this, -1);
}

bool TextureItem::TexturePool::clearItem( Inlist *pIt )
{
	TextureItem *pItem = (TextureItem *) pIt;
	if (pItem->used > 0) {
		return false;
	}

	if (pItem->pRef != NULL) {
		if (pItem->pRef->used == 0) {
			pItem->pRef->dispose();
			return false;
		}
		pItem->pRef->used--;
		return false;
	}

	pItem->loaded = false;
	if (m_bUseBitmapPool) {
		releaseBitmap(pItem);
	}
	return pItem->id >= 0;
}

void TextureItem::TexturePool::freeItem( Inlist *pIt )
{
	TextureItem *pItem = (TextureItem *) pIt;
	if (pItem->pRef == NULL && pItem->used == 0 && pItem->id >= 0) {
		m_nTexCnt--;
		
		//disposedTextures.add(pItem->id);
		pItem->id = -1;
		
	}
}

void TextureItem::TexturePool::releaseBitmap( TextureItem *pItem )
{
	if (pItem->pBitmap == NULL) {
		return;
	}
	m_pBitmaps->push_back(pItem->pBitmap);
	pItem->pBitmap = NULL;
}

void TextureItem::TexturePool::uploadTexture( TextureItem *pItem )
{
	if (pItem->pBitmap == NULL) {
		throw std::exception("Missing bitmap for texture");
	}
	if (pItem->id < 0) {
		int *textureIds = GLUtils::glGenTextures(1);
		pItem->id = textureIds[0];

		initTexture(pItem);

		if (DEBUG) {
		}

		m_nTexCnt++;
		pItem->pBitmap->uploadToTexture(false);
	} else {
		GLState::bindTex2D(pItem->id);
		pItem->pBitmap->uploadToTexture(true);
	}

	if (DEBUG) {
		GLUtils::checkGlError("TextureItem");
	}
	if (m_bUseBitmapPool) {
		releaseBitmap(pItem);
	}
}

void TextureItem::TexturePool::initTexture( TextureItem *pItem )
{
	GLState::bindTex2D(pItem->id);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (pItem->repeat) {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	} else {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

TextureItem::TexturePool* TextureItem::NOPOOL = new TextureItem::TexturePool(0);
std::vector<int> TextureItem::disposedTextures;

TextureItem::TextureItem( Bitmap *pBmp, bool repeat /*= false*/ )	
{
	init(NOPOOL, -1, pBitmap->getWidth(), pBitmap->getHeight(), repeat);
	this->pBitmap = pBmp;
}

TextureItem::TextureItem( TexturePool *pPool, int id ) 
{	
	init(pPool, id, pPool->m_nWidth, pPool->m_nHeight, false);	
}

void TextureItem::init( TexturePool *pPool, int id, int w, int h, bool repeat )
{
	this->id = id;
	this->width = w;
	this->height = h;
	this->pPool = pPool;
	this->repeat = repeat;

	this->pBitmap = NULL;
	this->pRef = NULL;	
	this->used = 0;
	this->loaded = false;
	this->offset = 0;
	this->indices = 0;
}


TextureItem::~TextureItem()
{
	if (pRef != NULL) {
		delete pRef;
		pRef = NULL;
	}

	if (pBitmap != NULL) {
		delete pBitmap;
		pBitmap = NULL;
	}

	if (pPool != NULL) {
		delete pPool;
		pPool = NULL;
	}
}

void TextureItem::upload()
{
	if (loaded)
		return;
	if (pRef == NULL) {
		pPool->uploadTexture(this);
	} else {
		pRef->upload();
		id = pRef->id;
	}
	loaded = true;
}

void TextureItem::bind()
{
	if (loaded) {
		GLState::bindTex2D(id);
	} else {
		upload();
	}
}

TextureItem* TextureItem::dispose() {
	TextureItem *pItem = (TextureItem *)this->next;
	this->next = NULL;
	pPool->release(this);
	return pItem;
}

void TextureItem::disposeTextures()
{
	int size = disposedTextures.size();
	if (size > 0) {
		int *tmp = new int[size];
		for (int i = 0; i < size; i++) {
			tmp[i] = disposedTextures.at(i);
		}
		disposedTextures.clear();
		GLUtils::glDeleteBuffers(size, tmp);
	}
}

TextureItem* TextureItem::clone( TextureItem *pItem )
{
	TextureItem *pClone = new TextureItem(NOPOOL, pItem->id);		
	pClone->repeat = pItem->repeat;	
	pClone->pRef = (pItem->pRef == NULL) ? pItem : pItem->pRef;
	pClone->loaded = pItem->loaded;
	pClone->pRef->used++;

	return pClone;
}
