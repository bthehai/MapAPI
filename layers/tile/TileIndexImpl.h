#pragma once
#include "..\..\utils\quadtree\TreeNode.h"
#include "..\..\utils\quadtree\TileIndex.h"

class MapTile;

class TileNode : public TreeNode<MapTile*> {
public:
	TileNode() 
		: TreeNode<MapTile*>()
	{
	}

	virtual ~TileNode() {
	}
};



class TileIndexMapTileImpl : public TileIndex<MapTile*> {

public:
	TileIndexMapTileImpl();
	virtual ~TileIndexMapTileImpl();

public:
	virtual TreeNode<MapTile*>* create();
	virtual void removeItem(MapTile* pItem);
};

