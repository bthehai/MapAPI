#pragma once

#include "..\Basic.h"
#include "Layers.h"
#include "ViewController.h"
#include "..\event\EventDispatcher.h"
#include "..\utils\async\TaskQueue.h"
#include "..\utils\async\AsyncExecutor.h"
#include "..\event\EventListener.h"

class Event;
class MotionEvent;
class MapPosition;
class Gesture;
class Animator;
class MapEventLayer;

class InputEventDispatcher : public EventDispatcher {
public:
	virtual void tell( EventListener *pListener, Event *pEvent, void* data ); 
};


class EventDispatcherImpl : public EventDispatcher {
public:
	virtual void tell( EventListener *pListener, Event *pEvent, void* data );
};

class Map : public TaskQueue {
public:
	static Event* POSITION_EVENT;
	static Event* UPDATE_EVENT;
	static Event* CLEAR_EVENT;

public:
	InputEventDispatcher *pInput;
	EventDispatcherImpl *pEvents;

private:
	Layers *m_pLayers;
	ViewController *m_pViewport;
	Animator *m_pAnimtor;

	MapPosition *m_pMapPosition;
	AsyncExecutor *m_pAsyncExecutor;
	MapEventLayer *m_pEventLayer;


public:
	Map();
	virtual ~Map();

public:
	bool handleGesture(Gesture *pGesture, MotionEvent *pMontionEvent);

	Animator* animator();
	Layers* layers();
	ViewController* viewport();

public:
	virtual bool post(Runnable *pTask) = 0;	
	virtual void render() = 0;
	virtual void updateMap(bool redraw) = 0;

	virtual int getWidth() = 0;
	virtual int getHeight() = 0;

public:
	virtual void addTask(Runnable *pTask);
};