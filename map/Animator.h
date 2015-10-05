#pragma once
#include "..\core\MapPosition.h"
#include "..\core\Point.h"
#include "..\core\BoundingBox.h"
#include "..\core\GeoPoint.h"
#include "ViewController.h"

class Map;

class Animator {
private:
	enum {
		ANIM_NONE = 0,
		ANIM_MOVE = 1 << 0,
		ANIM_SCALE = 1 << 1,
		ANIM_ROTATE = 1 << 2,
		ANIM_TILT = 1 << 3,
		ANIM_FLING = 1 << 4,

		DURATION_DEFAULT = 500,

	};

private:
	Map *m_pMap;
	MapPosition *m_pCurPos;
	MapPosition *m_pStartPos;
	MapPosition *m_pDeltaPos;

	Point m_Scroll;
	Point m_Pivot;
	Point m_Velocity;

	float m_Duration;
	long m_nAnimEnd;

	int m_nState;

public:
	Animator(Map *pMap);
	virtual ~Animator();

public:
	void animateTo(const BoundingBox& bbox, long duration = 500);
	void animateTo(const GeoPoint& p, double scale = 1.0, bool relative = true, long duration = 500);
	void animateTo(MapPosition *pMapPosition, long duration = 500);
	void animateZoom(double scaleBy, float pivotX, float pivotY, long duration = 500);
	void animateFling(float velocityX, float velocityY, int xmin, int xmax, int ymin, int ymax);

	void updateAnimation();
	void cancel();
private:
	void animStart(long duration, int state);
	void animCancel();
	double doScale(ViewController *pViewController, float adv);

};