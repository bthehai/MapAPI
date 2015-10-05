#pragma once
#include "Gesture.h"
#include "MotionEvent.h"

class GestureListener {
public:
	virtual bool onGesture(Gesture *pGesture, MotionEvent *pMotionEvent) = 0;
};