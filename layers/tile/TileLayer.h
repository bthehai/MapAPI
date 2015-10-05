#pragma once
#include "..\Layer.h"

#ifndef NUM_LOADERS
#define NUM_LOADERS 4
#endif

class TileLayer : public Layer, public Map::UpdateListener {

protected:
	const TileManager *m_pTileManager;
};