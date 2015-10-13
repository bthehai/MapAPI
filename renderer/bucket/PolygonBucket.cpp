#include "stdafx.h"
#include "PolygonBucket.h"
#include "..\GLViewport.h"
#include "..\GLMatrix.h"

RenderBucket* PolygonBucket::Renderer::draw( RenderBucket *pBuckets, GLViewport *pViewport, float div, bool first )
{
	throw std::exception("The method or operation is not implemented.");
}

void PolygonBucket::Renderer::clip( GLMatrix *pMVP, int clipMode )
{
	throw std::exception("The method or operation is not implemented.");
}

void PolygonBucket::Renderer::drawOver( GLMatrix *pMVP, int color, float alpha )
{
	throw std::exception("The method or operation is not implemented.");
}
