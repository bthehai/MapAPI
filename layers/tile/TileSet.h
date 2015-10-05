#pragma once
#include "..\..\Basic.h"

class MapTile;

class CoordComparator : public Comparator<MapTile*> {
public:
	virtual int compare( MapTile* lhs, MapTile* rhs );
};

class TileSet {
public:
	int count;
	MapTile** pTiles;
	int serial;

	static Comparator<MapTile*> *pCoordComparator;

private:
	int size;

public:
	int getSize() {
		return size;
	}

public:
	TileSet(int numTiles = 1);
	virtual ~TileSet();

private:
	void destroy();

public:
	MapTile* getTile(int x, int y);
	void lockTiles();
	void releaseTiles();
	void setTiles(TileSet *pSource);
	bool contains(MapTile *pTile);
};