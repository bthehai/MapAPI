#include "stdafx.h"
#include "GestureDetector.h"
#include "Gesture.h"
#include "MotionEvent.h"
#include "..\map\Map.h"

GestureDetector::GestureDetector( Map *pMap )
	: m_pMap(pMap)
{
}

GestureDetector::~GestureDetector()
{
	m_pMap = NULL;
}

bool GestureDetector::onTouchEvent( MotionEvent *pMotionEvent )
{
	if (pMotionEvent->getAction() == MotionEvent::ACTION_DOWN) {
		return m_pMap->handleGesture(Gesture::PRESS, pMotionEvent);
	}
	return false;
}