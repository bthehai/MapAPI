#pragma once

class MapTile;

class JobQueue {
private:
	int m_nCurrentJob;
	MapTile **m_pJobs;
	int m_nSize;

public:
	JobQueue();
	virtual ~JobQueue();

public:
	void setJobs(MapTile** pTiles, int size);
	void clear();
	bool isEmpty();
	MapTile* poll();
};