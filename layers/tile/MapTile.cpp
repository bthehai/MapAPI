#include "stdafx.h"
#include "MapTile.h"
#include "..\..\renderer\bucket\RenderBuckets.h"
#include "TileIndexImpl.h"



MapTile::MapTile(TileNode *pNode, int tx, int ty, int l) 
	: Tile(tx, ty, l)
	, x((double) (tileX / (1 << zoomLevel)))
	, y((double) (tileY / (1 << zoomLevel)))
{
	state_ = NONE;	
	this->pNode = pNode;
	pData = NULL;	
	distance = 0.0f;
	isVisible = true;
	fadeTime = 0;
	lastDraw = 0;
	proxy = 0;
	locked = 0;
	refs = 0;
}

MapTile::~MapTile() {

}

bool MapTile::state(int testState) {
	return (state_ & testState) != 0;
}

void MapTile::setState( byte newState )
{
	if (state_ == newState)
		return;

	if (state_ == DEADBEEF)
		return;

	char error[1024];
	switch (newState) {
	case NONE:
		state_ = newState;
		return;
	case LOADING:
		if (state_ == NONE) {
			state_ = newState;
			return;
		}		
		sprintf_s(error, "Loading <= %s %d", getStateName(), this); 
		throw std::exception(error);
	case NEW_DATA:
		if (state_ == LOADING) {
			state_ = newState;
			return;
		}
		sprintf_s(error, "NewData <= %s %d", getStateName(), this); 
		throw std::exception(error);
	case READY:
		if (state_ == NEW_DATA) {
			state_ = newState;
			return;
		}
		sprintf_s(error, "Ready <= %s %d", getStateName(), this); 
		throw std::exception(error);
	case CANCEL:
		if (state_ == LOADING) {
			state_ = newState;
			return;
		}
		sprintf_s(error, "Cancel <= %s %d", getStateName(), this); 
		throw std::exception(error);
	case DEADBEEF:
		state_ = newState;
		return;
	}
}

int MapTile::getState() {
	return state_;
}

std::string MapTile::getStateName()
{
	switch (state_) {
	case NONE:
		return "None";
	case LOADING:
		return "Loading";
	case NEW_DATA:
		return "Data";
	case READY:
		return "Ready";
	case CANCEL:
		return "Cancel";
	case DEADBEEF:
		return "Dead";
	}
	return "";
}


bool MapTile::isLocked() {
	return locked > 0 || refs > 0;
}

void MapTile::lock() {
	if (state_ == DEADBEEF) {
		return;
	}

	if (locked++ > 0) {
		return;
	}

	MapTile *p;
	for (int i = 0; i < 4; i++) {
		p = (MapTile *)pNode->child(i);
		if (p == NULL)
			continue;
		if (p->state(READY | NEW_DATA)) {
			proxy |= (1 << i);
			p->refs++;
		}
	}

	if (pNode->isRoot())
		return;

	p = (MapTile *)pNode->parent();
	if (p != NULL && p->state(READY | NEW_DATA)) {
		proxy |= PROXY_PARENT;
		p->refs++;
	}

	if (pNode->pParent->isRoot())
		return;
	p = (MapTile *)pNode->pParent->parent();
	if (p != NULL && p->state(READY | NEW_DATA)) {
		proxy |= PROXY_GRAMPA;
		p->refs++;
	}
}

void MapTile::unlock() {
	if (--locked > 0)
		return;

	TileNode *pParent = (TileNode *)pNode->pParent;
	if ((proxy & PROXY_PARENT) != 0) {
		MapTile *pItem = (MapTile *)pParent->item;		
		pItem->refs--;
	}

	if ((proxy & PROXY_GRAMPA) != 0) {
		pParent = (TileNode *)pParent->pParent;
		MapTile *pItem = (MapTile *)pParent->item;		
		pItem->refs--;
		//pParent->item->refs--;
	}
	for (int i = 0; i < 4; i++) {
		if ((proxy & (1 << i)) != 0) {
			MapTile *pMTile = (MapTile *)pNode->child(i);
			pMTile->refs--;
		}
	}

	/* removed all proxy references for this tile */
	proxy = 0;

	if (state_ == DEADBEEF) {		
		clear();
	}
}

bool MapTile::isActive() {
	return state_ > NONE;
}

bool MapTile::hasProxy(int proxy) {
	return (this->proxy & proxy) != 0;
}

void MapTile::clear() {
	TileData *pTemp;
	while (pData != NULL) {
 		pData->dispose();
		pTemp = pData;
		pData = (TileData *)pData->next;

		if (pTemp != NULL) {
			delete pTemp;
			pTemp = NULL;
		}
	}
	state(NONE);
}

RenderBuckets* MapTile::getBuckets()
{
	if (RenderBuckets *pRBuckets = dynamic_cast<RenderBuckets*>(pData)) {
		return pRBuckets;
	}
	return NULL;
}

TileData* MapTile::getData( void* id )
{
	for (Inlist *pIt = (Inlist *)pData; pIt != NULL; pIt = pIt->next) {
		TileData *pTemp = (TileData *)pIt;
		if (pTemp->id == id) {
			return pTemp;
		}
	}
	return NULL;
}

void MapTile::addData( void* id, TileData* pTileData )
{
	pTileData->id = id;
	if (pData != NULL) {
		pTileData->next = pData->next;
		pData->next = pTileData;
	} else {
		pData = pTileData;
	}
}

TileData* MapTile::removeData( void* id )
{
	if (pData == NULL) {
		return NULL;
	}
	TileData *pPrev = pData;
	if (pData->id == id) {
		pData = (TileData *)pData->next;
		return pPrev;
	}
	for (Inlist *pIt = pData->next; pIt != NULL; pIt = pIt->next) {
		TileData *pTemp = (TileData *)pIt;
		if (pTemp->id == id) {
			pPrev->next = pTemp->next;
			return pTemp;
		}
		pPrev = pTemp;
	}
	return NULL;
}

int MapTile::depthOffset( MapTile *pTile )
{
	return ((pTile->tileX % 4) + (pTile->tileY % 4 * 4) + 1);
}

MapTile* MapTile::getProxyChild( int id, byte state )
{
	if ((proxy & (1 << id)) == 0) {
		return NULL;
	}

	MapTile *pChild = (MapTile *)pNode->child(id);
	if (pChild == NULL || (pChild->state_ & state) == 0)
		return NULL;

	return pChild;
}

MapTile* MapTile::getParent()
{
	if ((proxy & PROXY_PARENT) == 0)
		return NULL;

	return (MapTile *)pNode->pParent->item;
}

MapTile* MapTile::getProxy( int proxy, byte state )
{
	if ((this->proxy & proxy) == 0)
		return NULL;

	MapTile *pTile = NULL;
	switch (proxy) {
	case PROXY_CHILD00:
		pTile = (MapTile *)pNode->child(0);
		break;
	case PROXY_CHILD01:
		pTile = (MapTile *)pNode->child(1);
		break;
	case PROXY_CHILD10:
		pTile = (MapTile *)pNode->child(2);
		break;
	case PROXY_CHILD11:
		pTile = (MapTile *)pNode->child(3);
		break;
	case PROXY_PARENT:
		pTile = (MapTile *)pNode->parent();
		break;
	case PROXY_GRAMPA:
		pTile = (MapTile *)pNode->pParent->parent();
		break;
	case PROXY_HOLDER:
		pTile = pHolder;
		break;
	}

	if (pTile == NULL || (pTile->state_ & state) == 0)
		return NULL;

	return pTile;
}

