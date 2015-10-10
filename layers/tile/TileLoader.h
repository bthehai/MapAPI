#pragma once

#include "..\..\utils\PausableThread.h"
#include "..\..\tiling\ITileDataSink.h"

class TileManager;
class MapTile;
class MapElement;
class Bitmap;

class TileLoader : public PausableThread, public ITileDataSink {

private:
	static int id;
	char THREAD_NAME[1024];
	TileManager *m_pTileManager;

protected:
	MapTile *m_pTile;

public:
	TileLoader(TileManager *pTileManager);
	virtual ~TileLoader();

public:
	 void go();

protected:
	virtual void doWork();
	virtual char* getThreadName();
	virtual int getThreadPriority();
	virtual bool hasWork();

	virtual bool loadTile(MapTile *pTile) = 0;

public:
	virtual void dispose() = 0;
	virtual void cancel() = 0;

	virtual void completed(QueryResult result);

	virtual void process(MapElement *pElement);
	virtual void setTileImage(Bitmap *pBitmap);
};