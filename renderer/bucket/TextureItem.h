#pragma once

#include "..\..\utils\pool\SyncPool.h"
#include "..\..\utils\pool\Inlist.h"
#include "..\..\backend\canvas\Bitmap.h"
#include <vector>
#include <list>


class TextureItem : public Inlist {

public:
	class TexturePool : public SyncPool {
	private:
		std::vector<Bitmap*> *m_pBitmaps;

		int m_nHeight;
		int m_nWidth;
		bool m_bUseBitmapPool;

	protected:
		int m_nTexCnt;

	public:
		TexturePool(int maxFill, int width, int height);
		TexturePool(int maxFill);
		virtual ~TexturePool();

	public:
		virtual Inlist* releaseAll( Inlist *pItem );
		virtual Inlist* get();
		virtual Inlist* get(Bitmap *pBmp);
		virtual Inlist* createItem();
		virtual bool clearItem(Inlist *pIt);
		virtual void freeItem(Inlist *pIt);

	protected:
		void releaseBitmap(TextureItem *pItem);
		void initTexture(TextureItem *pItem);

	private:
		void uploadTexture(TextureItem *pItem);

		friend class TextureItem;
	};

public:
	static TexturePool* NOPOOL;
	static std::vector<int> disposedTextures;

public:	
	int width;
	int height;
	bool repeat;

	int offset;
	int indices;

	Bitmap* pBitmap;

private:
	int id;
	int used;
	TextureItem* pRef;
	
	bool loaded;
	TexturePool *pPool;

private:
	TextureItem(TexturePool *pPool, int id);
	void init(TexturePool *pPool, int id, int w, int h, bool repeat);

public:
	TextureItem(Bitmap *pBmp, bool repeat = false);	
	virtual ~TextureItem();

public:
	TextureItem* dispose();
	void upload();
	void bind();

public:
	static void disposeTextures();
	static TextureItem* clone(TextureItem *pItem);
};