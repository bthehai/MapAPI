#include "stdafx.h"
#include "Gesture.h"


Gesture* Gesture::PRESS = new Press();

Gesture* Gesture::LONG_PRESS = new LongPress();

Gesture* Gesture::TAP = new Tap();

Gesture* Gesture::DOUBLE_TAP = new DoubleTap();
