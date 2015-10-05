#pragma once
#include "canvas\Canvas.h"

class CanvasAdapter {
public:
	static CanvasAdapter *pG;
	
	static float dpi;
	static float textScale;

protected:
	virtual Canvas* newCanvasImpl() = 0;
	virtual Paint* newPaintImpl() = 0;
	virtual Bitmap* newBitmapImpl(int width, int height, int format) = 0;
	virtual Bitmap* decodeBitmapImpl(void* pInputStream) = 0;
	virtual Bitmap* getBitmapAssetImpl(std::string fileName) = 0;	

protected:

	static Bitmap* createBitmap(std::string src) {
		if (src == "" || src.empty()) {
			return NULL;
		}
		void* pInputStream = NULL;
		//void* pInputStream = AssetAdapter::pG->openFileAsStream(src);
		if (pInputStream == NULL) {
			return NULL;
		}
		Bitmap *pBmp = decodeBitmap(pInputStream);

		delete pInputStream;
		pInputStream = NULL;

		return pBmp;
	}

	static void init(CanvasAdapter* pAdapter) {
		pG = pAdapter;
	}

public:
	static Canvas* newCanvas() {
		return pG->newCanvasImpl();
	}

	static Paint* newPaint() {
		return pG->newPaintImpl();
	}

	static Bitmap* newBitmap(int width, int height, int format) {
		return pG->newBitmapImpl(width, height, format);
	}

	static Bitmap* decodeBitmap(void* pInputStream) {
		return pG->decodeBitmapImpl(pInputStream);
	}

	static Bitmap* getBitmapAsset(std::string fileName) {
		return pG->getBitmapAssetImpl(fileName);
	}

};