#pragma once
#include "..\core\MapPosition.h"
#include "..\event\EventListener.h"

class Event;

class UpdateListener : public EventListener {
public:
	virtual void onMapEvent(Event *pEvent, MapPosition *pMapPosition) = 0;
};