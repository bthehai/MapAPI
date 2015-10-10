#pragma once

class Tile;
class InputStream;
class OutputStream;

class TileReader {
public:
	virtual Tile* getTile() = 0;
	virtual InputStream* getInputStream() = 0;
};

class TileWritter {
public:
	virtual Tile* getTile() = 0;
	virtual OutputStream* getOutputStream() = 0;
	virtual void complete(bool &success) = 0;
};


class ITileCache {
public:
	virtual TileWritter* writeTile(Tile *pTile) = 0;
	virtual TileReader* getTile(Tile *pTile) = 0;
	virtual void setCacheSize(long size) = 0;
	
};