#include "stdafx.h"
#include "Map.h"
#include "..\core\MapPosition.h"
#include "..\event\EventDispatcher.h"
#include "..\event\Event.h"
#include "..\event\MotionEvent.h"
#include "..\layers\MapEventLayer.h"
#include "Animator.h"
#include "InputListener.h"
#include "UpdateListener.h"

Event* Map::CLEAR_EVENT = new Event();
Event* Map::UPDATE_EVENT = new Event();
Event* Map::POSITION_EVENT = new Event();

void InputEventDispatcher::tell( EventListener *pListener, Event *pEvent, void* data )
{
	InputListener *inputListener = (InputListener *)pListener;	
	MotionEvent *pMotionEvent = (MotionEvent *)data;
	inputListener->onInputEvent(pEvent, pMotionEvent);
}

void EventDispatcherImpl::tell( EventListener *pListener, Event *pEvent, void* data )
{
	UpdateListener *updateListener = (UpdateListener *)pListener;		
	MapPosition *pMapPosition = (MapPosition *)data;
	updateListener->onMapEvent(pEvent, pMapPosition);
}


Map::Map() {
	pInput = new InputEventDispatcher();
	pEvents = new EventDispatcherImpl();

	m_pViewport = new ViewController();
	m_pAnimtor = new Animator(this);
	m_pLayers = new Layers(this);

	m_pAsyncExecutor = new AsyncExecutor(4, this);
	m_pMapPosition = new MapPosition();

	m_pEventLayer = new MapEventLayer(this);
	m_pLayers->add(0, m_pEventLayer);
}

Map::~Map() {
	if (pInput != NULL) {
		delete pInput;
		pInput = NULL;
	}

	if (pEvents != NULL) {
		delete pEvents;
		pEvents = NULL;
	}

	if (m_pViewport != NULL) {
		delete m_pViewport;
		m_pViewport = NULL;
	}

	if (m_pAnimtor != NULL) {
		delete m_pAnimtor;
		m_pAnimtor = NULL;
	}

	if (m_pLayers != NULL) {
		delete m_pLayers;
		m_pLayers = NULL;
	}

	if (m_pAsyncExecutor != NULL) {
		delete m_pAsyncExecutor;
		m_pAsyncExecutor = NULL;
	}

	if (m_pMapPosition != NULL) {
		delete m_pMapPosition;
		m_pMapPosition = NULL;
	}
}

bool Map::handleGesture( Gesture *pGesture, MotionEvent *pMontionEvent )
{
	throw std::exception("The method or operation is not implemented.");
}

Animator* Map::animator()
{
	return m_pAnimtor;
}

Layers* Map::layers()
{
	return m_pLayers;
}

ViewController* Map::viewport()
{
	return m_pViewport;
}

void Map::addTask( Runnable *pTask )
{
	m_pAsyncExecutor->post(pTask);
}

