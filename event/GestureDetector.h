#pragma once

class Map;
class MotionEvent;

class GestureDetector {
private:
	Map *m_pMap;

public:
	GestureDetector(Map *pMap);
	virtual ~GestureDetector();

public:
	bool onTouchEvent(MotionEvent *pMotionEvent);
};