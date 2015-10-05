#include "stdafx.h"
#include "TileIndexImpl.h"
#include "MapTile.h"


TileIndexMapTileImpl::TileIndexMapTileImpl()
	: TileIndex<MapTile*>()
{
	init();
}

TileIndexMapTileImpl::~TileIndexMapTileImpl()
{

}


TreeNode<MapTile*>* TileIndexMapTileImpl::create() {
	return (TreeNode<MapTile*>*)new TileNode();
}

void TileIndexMapTileImpl::removeItem( MapTile* pItem ) {
	MapTile *pTile = (MapTile *)pItem;
	if (pTile->pNode == NULL) {
		throw std::exception("Already removed");
	}
	TreeNode<MapTile*> *pNode = pTile->pNode;
	remove(pNode);

	Delete(pTile->pNode->item);
}
