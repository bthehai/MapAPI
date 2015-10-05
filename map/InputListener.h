#pragma once
#include "..\event\EventListener.h"

class Event;
class MotionEvent;

class InputListener : public EventListener {
public:
	virtual void onInputEvent(Event *pEvent, MotionEvent *pMotionEvent) = 0;
};