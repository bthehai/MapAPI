#include "stdafx.h"
#include "TileLoader.h"
#include "TileManager.h"

int TileLoader::id = 0;

TileLoader::TileLoader( TileManager *pTileManager )
	: PausableThread()
{
	m_pTileManager = pTileManager;
	sprintf_s(THREAD_NAME, "TileLoader %d", id++);
}

TileLoader::~TileLoader()
{

}

void TileLoader::go()
{
	notify();
}

void TileLoader::doWork()
{
	m_pTile = m_pTileManager->getTileJob();

	if (m_pTile == NULL)
		return;

	try {
		loadTile(m_pTile);
	} catch (std::exception &e) {
		/*e.printStackTrace();*/
		completed(FAILED);
	}
}

char* TileLoader::getThreadName()
{
	return THREAD_NAME;
}

int TileLoader::getThreadPriority()
{
	return 1;
}

bool TileLoader::hasWork()
{
	return m_pTileManager->hasTileJobs();
}

void TileLoader::completed( QueryResult result )
{
	bool ok = (result == SUCCESS);
	if (ok && (isCanceled()	|| isInterrupted())) {
		ok = false;
	}
	m_pTileManager->jobCompleted(m_pTile, ok);
	m_pTile = NULL;
}

void TileLoader::process( MapElement *pElement )
{

}

void TileLoader::setTileImage( Bitmap *pBitmap )
{

}


