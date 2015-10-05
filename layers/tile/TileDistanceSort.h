#pragma once

class MapTile;

class TileDistanceSort {

private:
	static TileDistanceSort* INSTANCE;

private:
	TileDistanceSort();

public:
	static void sort(MapTile **a, int size, int lo, int hi);
};