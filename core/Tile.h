#pragma once

class Tile {
public:
	static int SIZE;

	const int tileX;
	const int tileY;
	const int zoomLevel;

public:
	Tile(int tx, int ty, int l) 
		: tileX(tx)
		, tileY(ty)
		, zoomLevel(l)
	{
	}

	virtual ~Tile() {}


public:
	bool operator == (const Tile &o) {
		if (tileX == o.tileX && tileY == o.tileY && zoomLevel == o.zoomLevel) {
			return true;
		}
		return false;
	}
};