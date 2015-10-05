#include "stdafx.h"
#include "JobQueue.h"
#include "MapTile.h"
#include "TileDistanceSort.h"

JobQueue::JobQueue()
{
	m_nCurrentJob = 0;
	m_pJobs = NULL;
}

JobQueue::~JobQueue()
{
	m_nCurrentJob = 0;
	m_pJobs = NULL;
}

void JobQueue::setJobs( MapTile** pTiles, int size )
{
	m_pJobs = pTiles;
	m_nSize = size;
	m_nCurrentJob = 0;
}

void JobQueue::clear()
{
	if (m_pJobs == NULL) {
		m_nCurrentJob = 0;
		return;
	}
	MapTile** pTiles = m_pJobs;
	for (int i = m_nCurrentJob; i < m_nSize; i++) {
		MapTile *pTile = pTiles[i];
		if (pTile->state(LOADING | CANCEL)) {
			pTile->state(NONE);
		} else {
			char error[1024];
			sprintf_s(error, "Wrong tile in queue %d %d", pTile, pTile->getState());
		}
		delete pTile;
		pTile = NULL;
	}
	delete[] m_pJobs;
	m_pJobs = NULL;
	m_nCurrentJob = 0;
	m_nSize = 0;

}

bool JobQueue::isEmpty()
{
	return m_nSize == 0;
}

MapTile* JobQueue::poll()
{
	if (m_pJobs == NULL) {
		return NULL;
	}
	if (m_nCurrentJob == 0) {
		if (m_nSize > 1) {
			TileDistanceSort::sort(m_pJobs, m_nSize, 0, m_nSize);
		}
	}

	MapTile *pTile = m_pJobs[m_nCurrentJob];
	m_pJobs[m_nCurrentJob] = NULL;

	if (++m_nCurrentJob == m_nSize) {
		m_pJobs = NULL;
	}
	return pTile;
}

