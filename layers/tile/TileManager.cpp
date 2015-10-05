#include "stdafx.h"
#include "TileManager.h"
#include "TileIndexImpl.h"
#include "..\..\event\Event.h"
#include "..\..\map\Map.h"
#include "JobQueue.h"
#include "..\..\utils\FastMath.h"
#include "TileSet.h"
#include "MapTile.h"
#include "..\..\core\MapPosition.h"
#include "..\..\utils\ScanBox.h"
#include "..\..\renderer\BufferObject.h"
#include "TileDistanceSort.h"


class ScanBoxTileManagerImpl : public ScanBox {

private:
	TileManager *m_pTileManager;

public:
	ScanBoxTileManagerImpl(TileManager *pTileManager) {
		m_pTileManager = pTileManager;
	}

	friend class TileManager;

protected:
	virtual void setVisible(int y, int x1, int x2) {
		MapTile** pTiles = m_pTileManager->m_pNewTiles->pTiles;
		int size = m_pTileManager->m_pNewTiles->getSize();
		int cnt = m_pTileManager->m_pNewTiles->count;

		int maxTiles = size;

		int xmax = 1 << m_nZoom;

		for (int x = x1; x < x2; x++) {
			MapTile *pTile = NULL;

			if (cnt == maxTiles) {
				//log.debug("too many tiles {}", maxTiles);
				break;
			}
			int xx = x;

			if (x < 0 || x >= xmax) {
				/* flip-around date line */
				if (x < 0)
					xx = xmax + x;
				else
					xx = x - xmax;

				if (xx < 0 || xx >= xmax)
					continue;
			}

			/* check if tile is already added */
			for (int i = 0; i < cnt; i++)
				if (pTiles[i]->tileX == xx && pTiles[i]->tileY == y) {
					pTile = pTiles[i];
					break;
				}

				if (pTile == NULL) {
					pTile = m_pTileManager->addTile(xx, y, m_nZoom);
					pTiles[cnt++] = pTile;
				}
		}
		m_pTileManager->m_pNewTiles->count = cnt;
	}

};

void EventDispatcherMapTileImpl::tell( EventListener *pEventListener, Event *pEvent, void* data )
{
	Listener *pListener = (Listener *)pEventListener;		
	MapTile *pMapTile = (MapTile *)data;
	pListener->onTileManagerEvent(pEvent, pMapTile);
}

JobCompletedEvent::JobCompletedEvent(TileManager *pTileManager, MapTile *pTile, bool success)
{
	this->pTileManager = pTileManager;
	this->pTile = pTile;
	this->success = success;
}

JobCompletedEvent::~JobCompletedEvent()
{
	pTileManager = NULL;
	pTile = NULL;
	success = false;
}

void JobCompletedEvent::run()
{
	if (success && pTile->state(LOADING)) {
		pTile->setState(NEW_DATA);
		pTileManager->pEvents->fire(TileManager::TILE_LOADED, pTile);
		pTileManager->m_nTilesToUpload++;
		return;		
	}
	if (pTile->state(DEADBEEF)) {
		pTile->clear();
		return;
	}
	pTile->clear();
}


Event * TileManager::TILE_LOADED = NULL;
Event * TileManager::TILE_REMOVED = NULL;

TileManager::TileManager(Map *pMap, int cacheLimit)
	: m_pMap(pMap)
	, m_nCacheLimit(cacheLimit)
	, m_nCacheReduce(0)
	, m_nMinZoom(0)
	, m_nMaxZoom(20)
	, m_ZoomTable(NULL)
	, m_nZoomTable(0)
	, m_nTilesCount(0)
	, m_nTilesEnd(0)
	, m_nTilesToUpload(0)
	, m_nUpdateSerial(0)
	, m_pTileLock(NULL)
	, m_pCurrentTiles(NULL)
	, m_pNewTiles(NULL)
	, m_bLoadParent(false)
	, m_nPrevZoomLevel(0)
	, m_dbLevelUpThreshold(1.0)
	, m_dbLevelDownThreshold(2.0)
{
	m_pViewport = pMap->viewport();

	m_pJobs = new Array<MapTile*>();	
	m_pTiles = new MapTile*[m_nCacheLimit];
	m_nTilesLength = m_nCacheLimit;
	m_pIndex = new TileIndexMapTileImpl();

	pJobQueue = new JobQueue();
	m_pScanBox = new ScanBoxTileManagerImpl(this);

	pEvents = new EventDispatcherMapTileImpl();
}


TileManager::~TileManager()
{
	if (m_pIndex != NULL) {
		delete m_pIndex;
		m_pIndex = NULL;
	}

	if (m_pJobs != NULL) {
		delete m_pJobs;
		m_pJobs = NULL;
	}

	if (m_pTiles != NULL) {
		delete []m_pTiles;
		m_pTiles = NULL;
	}

	if (m_pIndex != NULL) {
		delete m_pIndex;
		m_pIndex = NULL;
	}

	if (pJobQueue != NULL) {
		delete pJobQueue;
		pJobQueue = NULL;
	}

	if (m_pNewTiles != NULL) {
		delete m_pNewTiles;
		m_pNewTiles = NULL;
	}
	if (m_pCurrentTiles != NULL) {
		delete m_pCurrentTiles;
		m_pCurrentTiles = NULL;
	}
}

void TileManager::setZoomTable( int *zoomTable, int size )
{
	m_ZoomTable = zoomTable;
	m_nZoomTable = size;
}

void TileManager::setZoomThresholds( float down, float up )
{
	m_dbLevelDownThreshold = (double) clamp(down, 1.0f, 2.0f);
	m_dbLevelUpThreshold = (double) clamp(up, 1.0f, 2.0f);
}

MapTile* TileManager::getTile( int x, int y, int z )
{
	return m_pIndex->getTile(x, y, z);
}

MapTile* TileManager::getTile(int tileX, int tileY, byte zoomLevel)
{
	return m_pIndex->getTile(tileX, tileY, zoomLevel);
}

MapTile* TileManager::addTile( int x, int y, int zoomLevel )
{
	MapTile *pTile = m_pIndex->getTile(x, y, zoomLevel);

	if (pTile == NULL) {
		TileNode* pNode = (TileNode *)m_pIndex->add(x, y, zoomLevel);
		pTile = pNode->item = new MapTile(pNode, x, y, zoomLevel);
		pTile->state(LOADING);
		m_pJobs->add(pTile);
		addToCache(pTile);
	} else if (!pTile->isActive()) {
		pTile->state(LOADING);
		m_pJobs->add(pTile);
	}

	if (m_bLoadParent && (zoomLevel > m_nMinZoom) && (m_ZoomTable == NULL)) {
		/* prefetch parent */
		MapTile* pMapTile = pTile->pNode->parent();
		if (pMapTile == NULL) {
			TileNode* pNode = (TileNode *)m_pIndex->add(x >> 1, y >> 1, zoomLevel - 1);
			pMapTile = pNode->item = new MapTile(pNode, x >> 1, y >> 1, zoomLevel - 1);
			addToCache(pMapTile);
			/* this prevents to add tile twice to queue */
			pMapTile->state(LOADING);
			m_pJobs->add(pMapTile);
		} else if (!pMapTile->isActive()) {
			pMapTile->state(LOADING);
			m_pJobs->add(pMapTile);
		}
	}
	return pTile;
}

void TileManager::init()
{
	if (m_pCurrentTiles != NULL) {
		m_pCurrentTiles->releaseTiles();
	}

	m_pIndex->drop();

	for (int i = 0; i < m_nTilesEnd; i++) {
		MapTile *pTile = m_pTiles[i];
		if (pTile == NULL) {
			continue;
		}
		if (!pTile->isLocked()) {
			pTile->clear();
		}

		pTile->state(DEADBEEF);
		delete pTile;
		pTile = NULL;		
	}

	memset(m_pTiles, NULL, m_nCacheLimit * sizeof(MapTile*));
	m_nTilesEnd = 0;
	m_nTilesCount = 0;

	int num = max(m_pMap->getWidth(), m_pMap->getHeight());
	int size = Tile::SIZE >> 1;
	int numTiles = (num * num) / (size * size) * 4;

	if (m_pNewTiles != NULL) {
		delete m_pNewTiles;
		m_pNewTiles = NULL;
	}
	m_pNewTiles = new TileSet(numTiles);

	if (m_pCurrentTiles != NULL) {
		delete m_pCurrentTiles;
		m_pCurrentTiles = NULL;
	}
	m_pCurrentTiles = new TileSet(numTiles);
	
}

void gl_Sort(MapTile **pTiles, int offset, int size, Comparator<MapTile*> *pComparator) {
	for (int i = offset; i < size - 1; i++) {
		for (int j = i + 1; j < size; j++) {
			MapTile *a = pTiles[i];
			MapTile *b = pTiles[j];
			if (pComparator->compare(a, b) > 0) {
				MapTile *temp = a;
				a = b;
				b = temp;
			}
		}
	}
}

bool TileManager::update( MapPosition *pMapPosition )
{
	if (m_pNewTiles == NULL || m_pNewTiles->getSize() == 0) {
		m_nPrevZoomLevel = pMapPosition->zoomLevel;
		init();
	}	
	pJobQueue->clear();

	if (pMapPosition->zoomLevel < m_nMinZoom) {
		if (m_pCurrentTiles->count > 0 && pMapPosition->zoomLevel < m_nMinZoom - 4) {
			m_pCurrentTiles->releaseTiles();
		}

		return false;
	}

	int tileZoom = clamp(pMapPosition->zoomLevel, m_nMinZoom, m_nMaxZoom);

	if (m_ZoomTable == NULL) {
		double scaleDiv = pMapPosition->scale / (1 << tileZoom);
		m_bLoadParent = scaleDiv < 1.5;

		int zoomDiff = tileZoom - m_nPrevZoomLevel;
		if (zoomDiff == 1) {
			if (scaleDiv < m_dbLevelUpThreshold) {
				tileZoom = m_nPrevZoomLevel;
				m_bLoadParent = false;
			}
		} else if (zoomDiff == -1) {
			if (scaleDiv > m_dbLevelDownThreshold) {
				tileZoom = m_nPrevZoomLevel;
				m_bLoadParent = true;
			}
		}
	} else {
		m_bLoadParent = false;
		int match = 0;
		for (int i = 0; i < m_nZoomTable; i++) {
			int z = m_ZoomTable[i];
			if (z <= tileZoom && z > match) {
				match = z;
			}
		}

		if (match == 0) {
			return false;
		}
		tileZoom = match;
	}

	m_nPrevZoomLevel = tileZoom;
	m_pViewport->getMapExtents(m_MapPlane, (float) (Tile::SIZE >> 1));
	
	m_pNewTiles->count = 0;
	m_pScanBox->scan(pMapPosition->x, pMapPosition->y, pMapPosition->scale, tileZoom, m_MapPlane);

	MapTile** newTiles = m_pNewTiles->pTiles;
	int newCount = m_pNewTiles->count;

	MapTile** curTiles = m_pCurrentTiles->pTiles;
	int curCount = m_pCurrentTiles->count;

	bool changed = (newCount != curCount);

	gl_Sort(newTiles, 0, newCount, TileSet::pCoordComparator);

	if (!changed) {
		for (int i = 0; i < newCount; i++) {
			if (newTiles[i] != curTiles[i]) {
				changed = true;
				break;
			}
		}
	}

	if (changed) {
		//synchronized(m_pTileLock) {
			m_pNewTiles->lockTiles();
			m_pCurrentTiles->releaseTiles();

			TileSet *pTmp = m_pCurrentTiles;
			m_pCurrentTiles = m_pNewTiles;
			m_pNewTiles = pTmp;

			m_nUpdateSerial++;
		//}

		m_pMap->render();
	}

	if (m_pJobs->isEmpty()) {
		return false;
	}

	MapTile** pJobs = m_pJobs->getData();
	int size = m_pJobs->getSize();

	updateDistances(pJobs, size, pMapPosition);

	pJobQueue->setJobs(pJobs, size);
	m_pJobs->empty();

	if (m_nCacheReduce < m_nCacheLimit / 2) {
		if (BufferObject::isMaxFill()) {
			m_nCacheReduce += 10;
			/*if (dbg)
				log.debug("reduce cache {}", (mCacheLimit - mCacheReduce));*/
		} else {
			m_nCacheReduce = 0;
		}
	}

	int remove = m_nTilesCount - (m_nCacheLimit - m_nCacheReduce);

	if (remove > CACHE_THRESHOLD || m_nTilesToUpload > MAX_TILES_IN_QUEUE) {
		limitCache(pMapPosition, remove);
	}

	return true;

}

void TileManager::addToCache( MapTile *pTile )
{
	if (m_nTilesEnd == m_nTilesLength) {
		if (m_nTilesEnd > m_nTilesCount) {
			TileDistanceSort::sort(m_pTiles, m_nTilesLength, 0, m_nTilesEnd);
			m_nTilesEnd = m_nTilesCount;
		}

		if (m_nTilesEnd == m_nTilesLength) {
			MapTile **pTmp = new MapTile*[m_nTilesLength + 20];
			m_nTilesLength += 20;

			memcpy(pTmp, m_pTiles, m_nTilesCount * sizeof(MapTile*));

			delete[] m_pTiles;
			m_pTiles = NULL;

			m_pTiles = pTmp;
		}
	}

	m_pTiles[m_nTilesEnd++] = pTile;	
	m_nTilesCount++;
}

bool TileManager::removeFromCache(MapTile *pTile)
{
	if (pTile->isLocked()) {
		return false;
	}

	if (pTile->state(NEW_DATA | READY))
		pEvents->fire(TILE_REMOVED, pTile);

	pTile->clear();
	m_pIndex->removeItem(pTile);
	m_nTilesCount--;

	delete pTile;
	pTile = NULL;

	return true;
}

void TileManager::limitCache( MapPosition* pPos, int remove )
{
	MapTile** pTiles = m_pTiles;
	int newTileCnt = 0;
	for (int i = 0; i < m_nTilesEnd; i++) {
		MapTile *pTile = pTiles[i];
		if (pTile == NULL) {
			continue;
		}

		if (pTile->state(NEW_DATA)) {
			newTileCnt++;
		}

		if (pTile->state(DEADBEEF)) {
			pTile->clear();
			delete pTiles[i];
			pTiles[i] = NULL;
			continue;
		}
		if (pTile->state(NONE) && removeFromCache(pTile)) {
			pTiles[i] = NULL;
			return;
		}
	}

	if (remove < CACHE_CLEAR_THRESHOLD && newTileCnt < MAX_TILES_IN_QUEUE) {
		return;
	}

	updateDistances(m_pTiles, m_nTilesEnd, pPos);
	TileDistanceSort::sort(pTiles, m_nTilesLength, 0, m_nTilesEnd);

	m_nTilesEnd = m_nTilesCount;

	for (int i = m_nTilesCount - 1; i >= 0 && remove > 0; i--) {
		MapTile *pTile = pTiles[i];

		if (pTile->isLocked()) {
			continue;
		}
		if (pTile->state(CANCEL)) {
			continue;
		}

		if (pTile->state(LOADING)) {
			pTile->setState(CANCEL);
			continue;
		}

		if (pTile->state(NEW_DATA)) {
			newTileCnt--;
		}

		if (!pTile->state(READY | NEW_DATA)) {

		}
		if (removeFromCache(pTile)) {
			pTiles[i] = NULL;
			remove--;
		}
	}

	for (int i = m_nTilesCount - 1; i >= 0 && newTileCnt > MAX_TILES_IN_QUEUE; i--) {
		MapTile *pTile = pTiles[i];
		if (pTiles != NULL && pTile->state(NEW_DATA)) {
			if (removeFromCache(pTile)) {
				pTiles[i] = NULL;
				newTileCnt--;
			}
		}
	}

	m_nTilesToUpload = newTileCnt;
}

void TileManager::updateDistances( MapTile** tiles, int size, MapPosition *pPos )
{
	int zoom = 20;
	long x = (long)(pPos->x * (1 << zoom));
	long y = (long)(pPos->y * (1 << zoom));

	for (int i = 0; i < size; i++) {
		MapTile *pTile = tiles[i];
		if (pTile == NULL) 
			continue;
		int diff = (zoom - pTile->zoomLevel);
		long dx, dy;

		if (diff == 0) {
			dx = pTile->tileX - x;
			dy = pTile->tileY - y;
		} else {
			long mx = x >> diff;
			long my = y >> diff;
			dx = pTile->tileX - mx;
			dy = pTile->tileY - my;
		}

		int dz = (pPos->zoomLevel - pTile->zoomLevel);
		if (dz == 0)
			dz = 1;
		else if (dz < -1)
			dz *= 0.75;

		pTile->distance = (dx * dx + dy * dy) * (dz * dz);
	}
}

MapTile* TileManager::getTileJob()
{
	return pJobQueue->poll();
}

bool TileManager::hasTileJobs()
{
	return !pJobQueue->isEmpty();
}

void TileManager::clearJobs()
{
	pJobQueue->clear();
}

bool TileManager::getActiveTiles(TileSet *pTileSet)
{
	if (m_pCurrentTiles == NULL) {
		return false;
	}

	if (pTileSet == NULL) {
		return false;
	}

	if (pTileSet->serial == m_nUpdateSerial) {
		return false;
	}

	//synchronized(m_pTileLock) {
	pTileSet->setTiles(m_pCurrentTiles);
	pTileSet->serial = m_nUpdateSerial;
	//}
	return true;
}

void TileManager::jobCompleted(MapTile *pTile, bool &success)
{
	Runnable *pRunnable = new JobCompletedEvent(this, pTile, success);
	m_pMap->post(pRunnable);

	if (pTile->isLocked()) {
		m_pMap->render();
	}
}

void TileManager::setZoomLevel(int zoomLevelMin, int zoomLevelMax)
{
	m_nMinZoom = zoomLevelMin;
	m_nMaxZoom = zoomLevelMax;
}


