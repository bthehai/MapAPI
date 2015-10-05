#pragma once

#include <string>
#include "Bitmap.h"
#include "Paint.h"

class Canvas {
public:
	virtual void setBitmap(Bitmap *pBitmap) = 0;
	virtual void drawText(std::string string, float x, float y, Paint *pFill, Paint *pStroke);
	virtual void drawBitmap(Bitmap *pBitmap, float x, float y) = 0;

};