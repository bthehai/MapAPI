#pragma once
#include "..\utils\pool\LList.h"


class Event;
class EventListener;


class EventDispatcher {

protected:
	LList<EventListener*>* m_pListeners;

public:
	void bind(EventListener *pListener);
	void unbind(EventListener *pListener);
	void fire(Event *pEvent, void* data);

public:
	virtual void tell(EventListener *pListener, Event *pEvent, void* data) = 0;
};

