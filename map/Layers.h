#pragma once

#include "..\layers\Layer.h"
#include "..\renderer\LayerRenderer.h"
#include "..\event\Gesture.h"
#include "..\event\MotionEvent.h"
#include "..\utils\array.h"

class Map;

class Layers {
private:
	Array<Layer*> m_LayerList;
	Map *m_pMap;

	bool m_bDirtyLayers;
	LayerRenderer **m_pLayerRenderers;
	Layer **m_pLayers;
	int m_nLayerRenderers;

public:
	Layers(Map *pMap);
	virtual ~Layers();

public:
	void setDirtyLayers(bool v);

	Layer* get(int index);
	Layer* remove(int index);

	int size();
	int rendererSize();

	bool add(Layer *pLayer);
	void add(int idx, Layer *pLayer);
	Layer* set(int idx, Layer *pLayer);
	LayerRenderer** getLayerRenderer();
	void destroy();
	bool handleGesture(Gesture *pGesture, MotionEvent *pMotionEvent);
	void updateLayers();
	
};