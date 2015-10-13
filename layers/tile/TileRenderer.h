#pragma once
#include "..\..\renderer\LayerRenderer.h"
#include "..\..\utils\ScanBox.h"

class TileManager;
class TileSet;
class MapTile;

class TileRenderer : public LayerRenderer {

	friend class ScanBoxTileRendererImpl;

public:
	enum {
		FADE_TIME = 500,
		MAX_TILE_LOAD = 8,
	};

private:
	TileManager *m_pTileManager;
	int m_nOverdraw;
	float m_fAlpha;
	int m_nUploadSerial;

	void *pTileLock;

protected:
	TileSet *m_pDrawTiles;
	int m_nProxyTileCnt;
	int m_nOverdrawColor;
	float m_fLayerAlpha;

public:
	TileRenderer();
	virtual ~TileRenderer();

public:
	void setTileManager(TileManager *pTileManager);

public:
	void setOverdrawColor(int color);
	void setBitmapAlpha(float alpha);
	void clearTiles();
	void releaseTiles(TileSet *pTileSet);

	bool getVisibleTiles(TileSet *pTileSet);


public:
	virtual void update(GLViewport *pViewport);

private:
	static int compileTileLayers(MapTile **pTiles, int tileCnt);
	static int uploadTileData(MapTile *pTile);

	ScanBox *m_pScanBox;

public:
	static long getMinFade(MapTile *pTile, int proxyLevel);
	
};