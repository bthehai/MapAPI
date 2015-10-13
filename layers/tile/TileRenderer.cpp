#include "stdafx.h"
#include "TileRenderer.h"
#include "TileSet.h"
#include "TileManager.h"
#include "MapTile.h"
#include "..\..\renderer\BufferObject.h"
#include "..\..\renderer\bucket\RenderBuckets.h"
#include "..\..\renderer\MapRenderer.h"
#include "TileIndexImpl.h"

class ScanBoxTileRendererImpl : public ScanBox {

	friend class TileManager;
	TileRenderer *pTileRenderer;

public:
	ScanBoxTileRendererImpl(TileRenderer *pTileRenderer) {
		this->pTileRenderer = pTileRenderer;
	}

protected:
	virtual void setVisible(int y, int x1, int x2);
};

TileRenderer::TileRenderer()
	: LayerRenderer()
	, m_pTileManager(NULL)
	, m_nOverdraw(0)
	, m_fAlpha(1)
	, m_pDrawTiles(new TileSet())
	, m_nProxyTileCnt(0)
	, m_nUploadSerial(0)
	, m_nOverdrawColor(0)
	, m_fLayerAlpha(1)
	, pTileLock(NULL)
{
	m_pScanBox = new ScanBoxTileRendererImpl(this);
}

TileRenderer::~TileRenderer() {

}

void TileRenderer::setTileManager( TileManager *pTileManager )
{
	m_pTileManager = pTileManager;
}

void TileRenderer::setOverdrawColor( int color )
{
	m_nOverdraw = color;
}

void TileRenderer::setBitmapAlpha( float alpha )
{
	m_fAlpha = alpha;
}

void TileRenderer::update( GLViewport *pViewport )
{
	if (m_fAlpha == 0) {
		m_pDrawTiles->releaseTiles();
		setReady(false);
		return;
	}

	m_fLayerAlpha = m_fAlpha;
	m_nOverdrawColor = m_nOverdraw;

	//synchronized(pTileLock) {
		bool tilesChanged = m_pTileManager->getActiveTiles(m_pDrawTiles);
		if (m_pDrawTiles->count == 0) {
			setReady(false);
			m_nProxyTileCnt = 0;
			return;
		}

		if (tilesChanged || pViewport->isChanged()) {
			m_nProxyTileCnt = 0;
			MapTile **pTiles = m_pDrawTiles->pTiles;
			int tileZoom = pTiles[0]->zoomLevel;

			for (int i = 0; i < m_pDrawTiles->count; i++) {
				pTiles[i]->isVisible = false;
			}

			m_pScanBox->scan(pViewport->pos.x, pViewport->pos.y, pViewport->pos.scale, tileZoom, pViewport->plane);
		}
	//}
		
	if (compileTileLayers(m_pDrawTiles->pTiles, m_pDrawTiles->count + m_nProxyTileCnt) > 0) {
		m_nUploadSerial++;
		BufferObject::checkBufferUsage(false);
	}
	setReady(true);
}

void TileRenderer::clearTiles()
{
	//synchronized(pTileLock) {
		m_pDrawTiles->releaseTiles();
		m_pDrawTiles->pTiles = new MapTile*[1];
		m_pDrawTiles->count = 0;
	//}
}

int TileRenderer::compileTileLayers( MapTile **pTiles, int tileCnt )
{
	int uploadCnt = 0;
	for (int i = 0; i < tileCnt; i++) {
		MapTile *pTile = pTiles[i];
		if (!pTile->isVisible) {
			continue;
		}

		if (pTile->state(READY)) {
			continue;
		}

		if (pTile->state(NEW_DATA)) {
			uploadCnt += uploadTileData(pTile);
			continue;
		}

		MapTile *pProxy = pTile->pHolder;
		if (pProxy != NULL && pProxy->state(NEW_DATA)) {
			uploadCnt += uploadTileData(pProxy);
			pTile->state_ = pProxy->state_;
			continue;
		}

		pProxy = pTile->getProxy(PROXY_PARENT, NEW_DATA);
		if (pProxy != NULL) {
			uploadCnt += uploadTileData(pProxy);
			continue;
		}

		for (int c = 0; c < 4; c++) {
			pProxy = pTile->getProxyChild(c, NEW_DATA);
			if (pProxy != NULL) {
				uploadCnt += uploadTileData(pProxy);
			}
		}

		if (uploadCnt >= MAX_TILE_LOAD) {
			break;
		}
	}
	return uploadCnt;
}

int TileRenderer::uploadTileData( MapTile *pTile )
{
	pTile->setState(READY);
	RenderBuckets *pBuckets = pTile->getBuckets();

	if (pBuckets == NULL) {
		return 0;
	}

	if (!pBuckets->compile(true)) {
		pBuckets->clear();
		return 0;
	}
	return 1;
}

bool TileRenderer::getVisibleTiles( TileSet *pTileSet )
{
	if (pTileSet == NULL) {
		return false;
	}

	if (m_pDrawTiles == NULL) {
		releaseTiles(pTileSet);
		return false;
	}

	int prevSerial = pTileSet->serial;

	//synchronized(pTileLock) {

		MapTile **pNewTiles = m_pDrawTiles->pTiles;
		int length = m_pDrawTiles->getSize();
		int cnt = m_pDrawTiles->count;
		
		if (pTileSet->getSize() != length) {
			pTileSet->releaseTiles();			
			pTileSet->pTiles = new MapTile*[length];
		}

		pTileSet->count = 0;
		for (int i = 0; i < cnt; i++) {
			MapTile *pTile = pNewTiles[i];
			if (pTile->isVisible && pTile->state(READY)) {
				pTile->lock();
			}
		}

		pTileSet->releaseTiles();

		for (int i = 0; i < cnt; i++) {
			MapTile *pTile = pNewTiles[i];
			if (pTile->isVisible && pTile->state(READY)) {
				pTileSet->pTiles[pTileSet->count++] = pTile;
			}
		}

		pTileSet->serial = m_nUploadSerial;
	//}

	return prevSerial != pTileSet->serial;
}

void TileRenderer::releaseTiles( TileSet *pTileSet )
{
	pTileSet->releaseTiles();
}

long TileRenderer::getMinFade( MapTile *pTile, int proxyLevel )
{
	long minFade = MapRenderer::frameTime - 50;
	/* check children for grandparent, parent or current */
	if (proxyLevel <= 0) {
		for (int c = 0; c < 4; c++) {
			MapTile *ci = pTile->pNode->child(c);
			if (ci == NULL)
				continue;

			if (ci->fadeTime > 0 && ci->fadeTime < minFade)
				minFade = ci->fadeTime;

			/* when drawing the parent of the current level
				* we also check if the children of current level
				* are visible */
			if (proxyLevel >= -1) {
				long m = getMinFade(ci, proxyLevel - 1);
				if (m < minFade)
					minFade = m;
			}
		}
	}

		/* check parents for child, current or parent */
		TileNode *p = (TileNode *)pTile->pNode->pParent;

		for (int i = proxyLevel; i >= -1; i--) {
			if (p == NULL)
				break;

			if (p->item != NULL && p->item->fadeTime > 0 && p->item->fadeTime < minFade)
				minFade = p->item->fadeTime;

			p = (TileNode *)p->pParent;
		}

		return minFade;
}

void ScanBoxTileRendererImpl::setVisible( int y, int x1, int x2 )
{
	MapTile **pTiles = pTileRenderer->m_pDrawTiles->pTiles;
	int length = pTileRenderer->m_pDrawTiles->getSize();
	int proxyOffset = pTileRenderer->m_pDrawTiles->count;

	for (int i = 0; i < proxyOffset; i++) {
		MapTile *pTile = pTiles[i];
		if (pTile->tileY == y && pTile->tileX >= x1 && pTile->tileX < x2)
			pTile->isVisible = true;
	}

	int xmax = 1 << m_nZoom;
	if (x1 >= 0 && x2 < xmax)
		return;

	O: for (int x = x1; x < x2; x++) {
		if (x >= 0 && x < xmax)
			continue;

		int xx = x;
		if (x < 0)
			xx = xmax + x;
		else
			xx = x - xmax;

		if (xx < 0 || xx >= xmax)
			continue;

		for (int i = proxyOffset; i < proxyOffset + pTileRenderer->m_nProxyTileCnt; i++)
			if (pTiles[i]->tileX == x && pTiles[i]->tileY == y) {
				goto O;
				//continue O;
			}

		MapTile *pTile = NULL;
		for (int i = 0; i < proxyOffset; i++)
			if (pTiles[i]->tileX == xx && pTiles[i]->tileY == y) {
				pTile = pTiles[i];
				break;
			}

			if (pTile == NULL)
				continue;

			if (proxyOffset + pTileRenderer->m_nProxyTileCnt >= length) {
				//log.error(" + mNumTileHolder");
				break;
			}

			MapTile *pHolder = new MapTile(NULL, x, y, (byte) m_nZoom);
			pHolder->isVisible = true;
			pHolder->pHolder = pTile;
			pHolder->state_ = pTile->state_;
			pTile->isVisible = true;
			pTiles[proxyOffset + pTileRenderer->m_nProxyTileCnt++] = pHolder;
	   }
}

