#include "stdafx.h"
#include "TileSet.h"
#include "MapTile.h"

TileSet::TileSet( int numTiles /*= 1*/ )
	: count(0)
	, pTiles(new MapTile*[numTiles])
	, serial(0)
{
	this->size = numTiles;
}

TileSet::~TileSet()
{
	destroy();
}

void TileSet::destroy() 
{
	if (pTiles != NULL) {
		for (int i = 0; i < size; i++) {
			MapTile *pTile = pTiles[i];
			if (pTile != NULL) {
				delete pTile;
				pTile = NULL;
			}
		}
		delete[] pTiles;
	}
}

MapTile* TileSet::getTile( int x, int y )
{
	for (int i = 0; i < count; i++) {
		if (pTiles[i]->tileX == x && pTiles[i]->tileY == y) {
			return pTiles[i];
		}
	}
	return NULL;
}

void TileSet::lockTiles()
{
	for (int i = 0; i < count; i++) {
		pTiles[i]->lock();
	}
}

void TileSet::releaseTiles()
{
	for (int i = 0; i < count; i++) {
		pTiles[i]->unlock();
	}
	destroy();
	count = 0;
	serial = 0;
}

void TileSet::setTiles(TileSet *pSource)
{
	pSource->lockTiles();
	releaseTiles();

	if (pSource->size != size) {
		pTiles = new MapTile*[pSource->size];
		size = pSource->size;
	}
	memcpy(pTiles, pSource->pTiles, size * sizeof(MapTile*));
	count = pSource->count;
}

bool TileSet::contains( MapTile *pTile )
{
	for (int i = 0; i < count; i++) {
		if (pTiles[i]->tileX == pTile->tileX 
			&& pTiles[i]->tileY == pTile->tileY)
			return true;
	}
	return false;
}

Comparator<MapTile*> * TileSet::pCoordComparator = new CoordComparator();

int CoordComparator::compare( MapTile* lhs, MapTile* rhs )
{
	if (lhs->tileX == rhs->tileX) {
		if (lhs->tileY == rhs->tileY)
			return 0;

		if (lhs->tileY < rhs->tileY)
			return 1;

		return -1;
	}
	if (lhs->tileX < rhs->tileX)
		return 1;

	return -1;
}
