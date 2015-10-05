#include "stdafx.h"
#include "MapEventLayer.h"

MapEventLayer::MapEventLayer( Map *pMap )
	: Layer(pMap)
{

}

MapEventLayer::~MapEventLayer()
{

}

void MapEventLayer::onInputEvent( Event *pEvent, MotionEvent *pMotionEvent )
{
	throw std::exception("The method or operation is not implemented.");
}

bool MapEventLayer::onGesture( Gesture *pGesture, MotionEvent *pMotionEvent )
{
	throw std::exception("The method or operation is not implemented.");
}

void MapEventLayer::onDetach()
{
	throw std::exception("The method or operation is not implemented.");
}
