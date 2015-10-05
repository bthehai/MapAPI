#include "stdafx.h"
#include "TileDistanceSort.h"
#include "MapTile.h"

TileDistanceSort* TileDistanceSort::INSTANCE = new TileDistanceSort();

TileDistanceSort::TileDistanceSort()
{

}

void TileDistanceSort::sort( MapTile **a, int size, int lo, int hi )
{
	for (int i = 0; i < size-1; i++) {
		for (int j = i + 1; j < size; j++) {
			if (a[i]->distance > a[j]->distance) {
				MapTile *pTemp = a[i];
				a[i] = a[j];
				a[j] = pTemp;
			}
		}
	}
	//throw std::exception("TileDistanceSort::sort");
}


