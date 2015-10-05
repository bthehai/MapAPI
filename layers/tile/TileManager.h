#pragma once
#include "..\..\utils\array.h"
#include "..\..\utils\quadtree\TileIndex.h"
#include "..\..\utils\quadtree\TreeNode.h"
#include "MapTile.h"
#include "..\..\event\EventListener.h"
#include "..\..\event\EventDispatcher.h"
#include "..\..\utils\thread\Runnable.h"

#define MAX_TILES_IN_QUEUE 20
#define CACHE_THRESHOLD 25
#define CACHE_CLEAR_THRESHOLD 10

class Event;
class Map;
class Viewport;
class MapTile;
class TileSet;
class JobQueue;
class MapPosition;
class ScanBox;
class ScanBoxTileManagerImpl;

template<class E>
class TileIndex;

class TileManager;

class Listener : public EventListener {
public:
	virtual void onTileManagerEvent(Event *pEvent, MapTile *pTile) = 0; 
};

class EventDispatcherMapTileImpl : public EventDispatcher {
public:
	virtual void tell( EventListener *pListener, Event *pEvent, void* data );
};

class JobCompletedEvent : public Runnable {

	friend class TileManager;

public:
	MapTile *pTile;
	bool success;
	TileManager *pTileManager;

public:
	JobCompletedEvent(TileManager *pTileManager, MapTile *pTile, bool success);
	virtual ~JobCompletedEvent();

public:
	virtual void run();

};

class TileManager {

	friend class ScanBoxTileManagerImpl;
	friend class JobCompletedEvent;

public:
	static Event *TILE_LOADED;
	static Event *TILE_REMOVED;

private:
	int m_nCacheLimit;
	int m_nCacheReduce;

	int m_nMinZoom;
	int m_nMaxZoom;

	int	*m_ZoomTable;
	int m_nZoomTable;

	Map *m_pMap;
	Viewport *m_pViewport;

	MapTile** m_pTiles;
	int m_nTilesLength;
	int m_nTilesCount;
	int m_nTilesEnd;
	int m_nTilesToUpload;

	Array<MapTile*>* m_pJobs;

	int m_nUpdateSerial;

	void* m_pTileLock;

	TileSet *m_pCurrentTiles;
	TileSet *m_pNewTiles;
	JobQueue *pJobQueue;
	float m_MapPlane[8];

	bool m_bLoadParent;
	int m_nPrevZoomLevel;

	double m_dbLevelUpThreshold;
	double m_dbLevelDownThreshold;

	TileIndex<MapTile*> *m_pIndex;	

	ScanBox *m_pScanBox;

public:
	EventDispatcher *pEvents;

public:
	TileManager(Map *pMap, int cacheLimit);
	virtual ~TileManager();

public:
	void init();

	void setZoomTable(int *zooTable, int size);
	void setZoomThresholds(float down, float up);

	void clearJobs();
	bool hasTileJobs();
	MapTile* getTileJob();

	MapTile* addTile(int x, int y, int zoomLevel);
	MapTile* getTile(int x, int y, int z);	
	MapTile* getTile(int tileX, int tileY, byte zoomLevel);

	void setZoomLevel(int zoomLevelMin, int zoomLevelMax);

	bool update(MapPosition *pMapPosition);	
	bool getActiveTiles(TileSet *pTileSet);

	void jobCompleted(MapTile *pTile, bool &success);

private:
	void addToCache(MapTile *pTile);
	bool removeFromCache(MapTile *pTile);
	void limitCache(MapPosition* pPos, int remove);


private:
	static void updateDistances(MapTile** tiles, int size, MapPosition *pPos);

	

};