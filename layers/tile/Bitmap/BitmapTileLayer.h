#pragma once
#include "..\TileLayer.h"

class TileSource;

class FadeStep {
public:
	double scaleStart, scaleEnd;
	float alphaStart, alphaEnd;

public:
	FadeStep(int zoomStart, int zoomEnd, float aStart, float aEnd) 
		: scaleStart(1 << zoomStart)
		, scaleEnd(1 << zoomEnd)
		, alphaStart(aStart)
		, alphaEnd(aEnd)
	{
	}
};

class BitmapTileLayer : public TileLayer {

private:
	enum {
		CACHE_LIMIT = 40,
	};

protected:
	TileSource *m_pTileSource;

public:
	BitmapTileLayer(Map *pMap, TileSource *pTileSource, int cacheLimit = CACHE_LIMIT);
	virtual ~BitmapTileLayer();
};