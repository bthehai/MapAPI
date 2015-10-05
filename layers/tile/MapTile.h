#pragma once
#include "..\..\core\Tile.h"
#include "..\..\utils\quadtree\TreeNode.h"
#include "..\..\utils\pool\Inlist.h"


enum State {
	NONE		= (1 << 0),
	LOADING		= (1 << 1),
	NEW_DATA	= (1 << 2),
	READY		= (1 << 3),
	CANCEL		= (1 << 4),
	DEADBEEF	= (1 << 6)
};

#define PROXY_CHILD00 1 << 0
#define PROXY_CHILD01 1 << 1
#define PROXY_CHILD10 1 << 2
#define PROXY_CHILD11 1 << 3
#define PROXY_PARENT  1 << 4
#define PROXY_GRAMPA  1 << 5
#define PROXY_HOLDER  1 << 6

class MapTile;
class RenderBuckets;
class TileNode;


class TileData : public Inlist {
public:
	void* id;

public:
	virtual void dispose() = 0;	
};


class MapTile : public Tile {
	
public:
	int state_;

	const double x;
	const double y;

	TileData *pData;
	TileNode *pNode;

	float distance;

	bool isVisible;

	long fadeTime;

	int lastDraw;

	int proxy;
	int locked;
	int refs;

	MapTile *pHolder;

public:
	MapTile(TileNode *pNode, int tx, int ty, int l);
	virtual ~MapTile();

public:
	bool state(int testState);
	int getState();

	bool isLocked();

	void lock();
	void unlock();

	bool isActive();
	bool hasProxy(int proxy);	

	RenderBuckets* getBuckets();
	TileData* getData(void* id);
	void addData(void* id, TileData* pTileData);
	TileData* removeData(void* id);

	MapTile* getProxyChild(int id, byte state);
	MapTile* getParent();
	MapTile* getProxy(int proxy, byte state);

	std::string getStateName();
	void setState(byte newState);

public:
	void clear();

public:
	static int depthOffset(MapTile *pTile);
};