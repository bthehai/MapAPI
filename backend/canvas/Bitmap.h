#pragma once
#include "..\..\Basic.h"

class Bitmap {
public:
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
	virtual void recycle() = 0;
	virtual int* getPixels() = 0;
	virtual void eraseColor(unsigned int color) = 0;
	virtual void uploadToTexture(bool replace) = 0;
	virtual bool isValid() = 0;
};