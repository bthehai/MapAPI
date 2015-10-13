#include "stdafx.h"
#include "BitmapTileLayer.h"
#include "..\TileManager.h"
#include "..\VectorTileRenderer.h"

BitmapTileLayer::BitmapTileLayer(Map *pMap, TileSource *pTileSource, int cacheLimit /* = CACHE_LIMIT*/)
	: TileLayer(
		pMap, 
		new TileManager(pMap, cacheLimit),
		new VectorTileRenderer())
{

}

BitmapTileLayer::~BitmapTileLayer()
{

}
