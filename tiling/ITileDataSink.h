#pragma once

class MapElement;
class Bitmap;

class ITileDataSink {

public:
	static enum QueryResult {
		SUCCESS,
		FAILED,
		TILE_NOT_FOUND,
		DELAYED,
	};

public:
	virtual void process(MapElement *pElement) = 0;
	virtual void setTileImage(Bitmap *pBitmap) = 0;
	virtual void completed(QueryResult result) = 0;
};