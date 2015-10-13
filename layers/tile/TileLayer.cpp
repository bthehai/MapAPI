#include "stdafx.h"
#include "TileLayer.h"
#include "..\..\map\Map.h"
#include "TileManager.h"
#include "TileRenderer.h"
#include "TileLoader.h"


TileLayer::TileLayer( Map *pMap, TileManager *pTileManager, TileRenderer *pRenderer /*= NULL*/ )
	: Layer(pMap)
	, m_pTileManager(pTileManager)
{
	if (pRenderer != NULL) {
		pRenderer->setTileManager(pTileManager);
	}
	m_pRenderer = pRenderer;
	m_pTileLoader = NULL;
	m_nSizeLoader = 0;
}

TileLayer::~TileLayer()
{
	if (m_pTileManager != NULL) {
		delete m_pTileManager;
		m_pTileManager = NULL;
	}

	if (m_pRenderer != NULL) {
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

void TileLayer::setRenderer( TileRenderer *pRenderer )
{
	if (m_pRenderer != NULL) {
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
	pRenderer->setTileManager(m_pTileManager);
	m_pRenderer = pRenderer;
}

TileRenderer* TileLayer::tileRenderer()
{
	return (TileRenderer *)m_pRenderer;
}

TileManager* TileLayer::getManager()
{
	return m_pTileManager;
}

void TileLayer::initLoader( int numLoaders )
{
	if (m_pTileLoader != NULL) {
		for (int i = 0; i < m_nSizeLoader; i++) {
			delete m_pTileLoader[i];
			m_pTileLoader[i] = NULL;
		}
		delete []m_pTileLoader;
	}

	m_pTileLoader = new TileLoader*[numLoaders];
	m_nSizeLoader = numLoaders;
	for (int i = 0; i < m_nSizeLoader; i++) {
		m_pTileLoader[i] = createLoader();
		m_pTileLoader[i]->start();
	}
}

int TileLayer::getNumLoaders()
{
	return NUM_LOADERS;
}

void TileLayer::onMapEvent( Event *pEvent, MapPosition *pMapPosition )
{
	if (pEvent == Map::CLEAR_EVENT) {
		//synchronized(m_pRenderer) {
			tileRenderer()->clearTiles();
			m_pTileManager->init();
		//}

		if (m_pTileManager->update(pMapPosition)) {
			notifyLoaders();
		}
	} else if (pEvent == Map::POSITION_EVENT) {
		if (m_pTileManager->update(pMapPosition)) {
			notifyLoaders();
		}
	}
}

void TileLayer::onDetach()
{
	for (int i = 0; i < m_nSizeLoader; i++) {
		TileLoader *pLoader = m_pTileLoader[i];
		if (pLoader != NULL) {
			pLoader->pause();
			pLoader->finish();
			pLoader->dispose();

// 			delete pLoader;
// 			pLoader = NULL;
		}
	}
}

void TileLayer::notifyLoaders()
{
	for (int i = 0; i < m_nSizeLoader; i++) {
		TileLoader *pLoader = m_pTileLoader[i];
		if (pLoader != NULL) {
			pLoader->go();
		}
	}
}

void TileLayer::pauseLoaders( bool wait )
{
	TileLoader *pLoader;
	int i;
	for (i = 0; i < m_nSizeLoader; i++) {
		pLoader = m_pTileLoader[i];
		pLoader->cancel();

		if (!pLoader->isPausing()) {
			pLoader->pause();
		}
	}

	if (!wait) {
		return;
	}

	for (i = 0; i < m_nSizeLoader; i++) {
		pLoader = m_pTileLoader[i];
		if (!pLoader->isPausing()) {
			pLoader->awaitPausing();
		}
	}
}

void TileLayer::resumeLoaders()
{
	TileLoader *pLoader;
	for (int i = 0; i < m_nSizeLoader; i++) {
		pLoader = m_pTileLoader[i];
		pLoader->proceed();
	}
}
