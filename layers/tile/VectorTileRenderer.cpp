#include "stdafx.h"
#include "VectorTileRenderer.h"
#include "MapTile.h"
#include "TileSet.h"
#include "..\..\utils\FastMath.h"
#include "..\..\renderer\MapRenderer.h"
#include "..\..\renderer\bucket\RenderBuckets.h"
#include "..\..\renderer\bucket\RenderBucket.h"
#include "..\..\renderer\bucket\PolygonBucket.h"
#include "..\..\renderer\bucket\LineBucket.h"
#include "..\..\renderer\bucket\MeshBucket.h"
#include "..\..\renderer\bucket\HairLineBucket.h"
#include "..\..\renderer\bucket\LineTexBucket.h"
#include "..\..\renderer\bucket\BitmapBucket.h"

bool debugOverdraw = false;

VectorTileRenderer::VectorTileRenderer()
	: TileRenderer()
{
	m_pClipProj = new GLMatrix();
	m_pClipMVP = new GLMatrix();
}

VectorTileRenderer::~VectorTileRenderer()
{
	delete m_pClipMVP;
	delete m_pClipProj;
}

void VectorTileRenderer::render( GLViewport *pViewport )
{
	m_pClipProj->copy(pViewport->proj);
	m_pClipProj->setValue(10, 0);
	m_pClipProj->setValue(14, 0);
	m_pClipProj->multiplyRhs(pViewport->view);

	m_nClipMode = PolygonBucket::CLIP_STENCIL;

	int tileCnt = m_pDrawTiles->count + m_nProxyTileCnt;
	int i;
	MapTile **pTiles = m_pDrawTiles->pTiles;
	MapTile *pTile;

	bool drawProxies = false;	

	m_nDrawSerial++;

	for (i = 0; i < tileCnt; i++) {
		pTile = pTiles[i];

		if (pTile->isVisible && !pTile->state(READY)) {
			glDepthMask(true);
			glClear(GL_DEPTH_BUFFER_BIT);

			glDepthFunc(GL_ALWAYS);

			m_nClipMode = PolygonBucket::CLIP_DEPTH;
			drawProxies = true;

			break;
		}
	}

	for (i = 0; i < tileCnt; i++) {
		pTile = pTiles[i];
		if (pTile->isVisible && pTile->state(READY)) {
			drawTile(pTile, pViewport, 0);
		}
	}

	if (!drawProxies) {
		return;
	}

	glDepthFunc(GL_LESS);

	bool preferParent = (pViewport->pos.getZoomScale() < 1.5) 
						|| (pViewport->pos.zoomLevel < pTiles[0]->zoomLevel);

	if (preferParent) {
		for (i = 0; i < tileCnt; i++) {
			pTile = pTiles[i];
			if (!pTile->isVisible || pTile->lastDraw == m_nDrawSerial) {
				continue;
			}

			if (!drawParent(pTile, pViewport)) {
				drawChildren(pTile, pViewport);
			}
		}
	} else {
		for (i = 0; i < tileCnt; i++) {
			pTile = pTiles[i];
			if (!pTile->isVisible || pTile->lastDraw == m_nDrawSerial) {
				continue;
			}
			drawChildren(pTile, pViewport);
		}

		for (i = 0; i < tileCnt; i++) {
			pTile = pTiles[i];
			if (!pTile->isVisible || pTile->lastDraw == m_nDrawSerial) {
				continue;
			}
			drawParent(pTile, pViewport);
		}
	}

	for (i = 0; i < tileCnt; i++) {
		pTile = pTiles[i];
		if (!pTile->isVisible || pTile->lastDraw == m_nDrawSerial) {
			continue;
		}
		drawGrandParent(pTile, pViewport);
	}

	glDepthMask(false);

}

void VectorTileRenderer::drawTile( MapTile *pTile, GLViewport *pViewport, int proxyLevel )
{
	if (pTile->lastDraw == m_nDrawSerial)
		return;

	pTile->lastDraw = m_nDrawSerial;

	/* use holder proxy when it is set */
	RenderBuckets *pBuckets = (pTile->pHolder == NULL)
		? pTile->getBuckets()
		: pTile->pHolder->getBuckets();

	if (pBuckets == NULL || pBuckets->pVbo == NULL) {
		//log.debug("{} no buckets!", tile);
		return;
	}

	MapPosition pos = pViewport->pos;
	/* place tile relative to map position */
	int z = pTile->zoomLevel;
	float div = pow(z - pos.zoomLevel);
	double tileScale = Tile::SIZE * pos.scale;
	float x = (float) ((pTile->x - pos.x) * tileScale);
	float y = (float) ((pTile->y - pos.y) * tileScale);

	/* scale relative to zoom-level of this tile */
	float scale = (float) (pos.scale / (1 << z));

	pViewport->mvp.setTransScale(x, y, scale / COORD_SCALE);
	pViewport->mvp.multiplyLhs(pViewport->viewproj);

	m_pClipMVP->setTransScale(x, y, scale / COORD_SCALE);
	m_pClipMVP->multiplyLhs(*m_pClipProj);

	pBuckets->bind();

	PolygonBucket::Renderer::clip(m_pClipMVP, m_nClipMode);
	bool first = true;

	for (RenderBucket *pRBucket = pBuckets->get(); pRBucket != NULL;) {
		switch (pRBucket->type) {
		case RenderBucket::POLYGON:
			pRBucket = PolygonBucket::Renderer::draw(pRBucket, pViewport, div, first);
			first = false;
			/* set test for clip to tile region */
			glStencilFunc(GL_EQUAL, 0x80, 0x80);
			break;
		case RenderBucket::LINE:
			pRBucket = LineBucket::Renderer::draw(pRBucket, pViewport, scale, pBuckets);
			break;
		case RenderBucket::TEXLINE:
			pRBucket = LineTexBucket::Renderer::draw(pRBucket, pViewport, div, pBuckets);
			break;
		case RenderBucket::MESH:
			pRBucket = MeshBucket::Renderer::draw(pRBucket, pViewport);
			break;
		case RenderBucket::HAIRLINE:
			pRBucket = HairLineBucket::Renderer::draw(pRBucket, pViewport);
			break;
		case RenderBucket::BITMAP:
			pRBucket = BitmapBucket::Renderer::draw(pRBucket, pViewport, 1, m_fLayerAlpha);
			break;
		default:
			/* just in case */
			//log.error("unknown layer {}", pRBucket.type);
			pRBucket = (RenderBucket *)pRBucket->next;
			break;
		}

		/* make sure buffers are bound again */
		pBuckets->bind();
	}

// 	if (debugOverdraw) {
// 		if (tile.zoomLevel > pos.zoomLevel)
// 			PolygonBucket.Renderer.drawOver(mClipMVP, Color.BLUE, 0.5f);
// 		else if (tile.zoomLevel < pos.zoomLevel)
// 			PolygonBucket.Renderer.drawOver(mClipMVP, Color.RED, 0.5f);
// 		else
// 			PolygonBucket.Renderer.drawOver(mClipMVP, Color.GREEN, 0.5f);
// 
// 		return;
// 	}

	long fadeTime = pTile->fadeTime;
	if (fadeTime == 0) {
		if (pTile->pHolder == NULL) {
			fadeTime = getMinFade(pTile, proxyLevel);
		} else {
			/* need to use time from original tile */
			fadeTime = pTile->pHolder->fadeTime;
			if (fadeTime == 0)
				fadeTime = getMinFade(pTile->pHolder, proxyLevel);
		}
		pTile->fadeTime = fadeTime;
	}

	long dTime = MapRenderer::frameTime - fadeTime;

	if (m_nOverdrawColor == 0 || dTime > FADE_TIME) {
		PolygonBucket::Renderer::drawOver(m_pClipMVP, 0, 1);
		return;
	}

	float fade = 1 - dTime / FADE_TIME;
	PolygonBucket::Renderer::drawOver(m_pClipMVP, m_nOverdrawColor, fade * fade);

	MapRenderer::animate();
}

bool VectorTileRenderer::drawChildren( MapTile *pTile, GLViewport *pViewport )
{
	int drawn = 0;
	for (int i = 0; i < 4; i++) {
		MapTile *c = pTile->getProxyChild(i, READY);
		if (c == NULL)
			continue;

		drawTile(c, pViewport, 1);
		drawn++;
	}
	if (drawn == 4) {
		pTile->lastDraw = m_nDrawSerial;
		return true;
	}
	return false;
}

bool VectorTileRenderer::drawParent( MapTile *pTile, GLViewport *pViewport )
{
	MapTile *pProxy = pTile->getProxy(PROXY_PARENT, READY);
	if (pProxy != NULL) {
		drawTile(pProxy, pViewport, -1);
		pTile->lastDraw = m_nDrawSerial;
		return true;
	}
	return false;
}

void VectorTileRenderer::drawGrandParent( MapTile *pTile, GLViewport *pViewport )
{
	MapTile *pProxy = pTile->getProxy(PROXY_GRAMPA, READY);
	if (pProxy != NULL) {
		drawTile(pProxy, pViewport, -2);
		pTile->lastDraw = m_nDrawSerial;
	}
}

