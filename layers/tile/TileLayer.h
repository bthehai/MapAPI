#pragma once
#include "..\Layer.h"
#include "..\..\map\UpdateListener.h"

class Map;
class TileManager;
class TileLoader;
class TileRenderer;


class TileLayer : public Layer, public UpdateListener {

protected:
	TileManager *m_pTileManager;
	TileLoader **m_pTileLoader;
	int m_nSizeLoader;

public:
	TileLayer(Map *pMap, TileManager *pTileManager, TileRenderer *pRenderer = NULL);
	virtual ~TileLayer();

	enum {
		NUM_LOADERS = 4,
	};

protected:
	virtual TileLoader* createLoader() = 0;

	void setRenderer(TileRenderer *pRenderer);
	void initLoader(int numLoaders);

	int getNumLoaders();

	void pauseLoaders(bool wait);
	void resumeLoaders();

public:
	TileRenderer* tileRenderer();
	TileManager* getManager();

	void notifyLoaders();


public:
	virtual void onMapEvent(Event *pEvent, MapPosition *pMapPosition);
	virtual void onDetach();

};