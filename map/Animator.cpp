#include "stdafx.h"
#include "Animator.h"
#include "Map.h"


Animator::Animator(Map *pMap)
{
	m_pMap = pMap;
	m_pCurPos = new MapPosition();
	m_pStartPos = new MapPosition();
	m_pDeltaPos = new MapPosition();

	m_Duration = DURATION_DEFAULT;
	m_nAnimEnd = -1;
	m_nState = ANIM_NONE;
}

Animator::~Animator()
{
	throw std::exception("The method or operation is not implemented.");
}

void Animator::animateTo(const BoundingBox& bbox, long duration /*= 500*/ )
{
	throw std::exception("The method or operation is not implemented.");
}

void Animator::animateTo( const GeoPoint& p, double scale /*= 1.0*/, bool relative /*= true*/, long duration /*= 500*/ )
{
	throw std::exception("The method or operation is not implemented.");
}

void Animator::animateTo( MapPosition *pMapPosition, long duration /*= 500*/ )
{
	throw std::exception("The method or operation is not implemented.");
}

void Animator::animateZoom( double scaleBy, float pivotX, float pivotY, long duration /*= 500*/ )
{
	throw std::exception("The method or operation is not implemented.");
}

void Animator::animateFling( float velocityX, float velocityY, int xmin, int xmax, int ymin, int ymax )
{
	throw std::exception("The method or operation is not implemented.");
}

void Animator::updateAnimation()
{
	throw std::exception("The method or operation is not implemented.");
}

void Animator::cancel()
{
	throw std::exception("The method or operation is not implemented.");
}

void Animator::animStart( long duration, int state )
{
	throw std::exception("The method or operation is not implemented.");
}

void Animator::animCancel()
{
	throw std::exception("The method or operation is not implemented.");
}

double Animator::doScale( ViewController *pViewController, float adv )
{
	throw std::exception("The method or operation is not implemented.");
}

