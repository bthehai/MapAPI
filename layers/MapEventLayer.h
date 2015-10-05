#pragma once

#include "Layer.h"
#include "..\event\GestureListener.h"
#include "..\map\InputListener.h"

class Map;

class MapEventLayer : public Layer, public InputListener, public GestureListener {

public:
	MapEventLayer(Map *pMap);
	virtual ~MapEventLayer();

public:
	virtual void onInputEvent( Event *pEvent, MotionEvent *pMotionEvent );
	virtual bool onGesture( Gesture *pGesture, MotionEvent *pMotionEvent );

	virtual void onDetach();

};