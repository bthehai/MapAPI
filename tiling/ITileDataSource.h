#pragma once

class MapTile;
class ITileDataSink;

class ITileDataSource {

public:
	virtual void query(MapTile *pTile, ITileDataSink *pMapDataSink) = 0;
	virtual void dispose() = 0;
	virtual void cancel() = 0;
};