#include "stdafx.h"
#include "RenderBuckets.h"
#include "..\..\core\Tile.h"
#include "..\MapRenderer.h"

RenderBuckets::RenderBuckets()
	: pVbo(NULL)
	, pIbo(NULL)
	, m_pCurBucket(NULL)
	, m_pBuckets(NULL)
{	
	offset[0] = 0;
	offset[1] = 1;
}

RenderBuckets::~RenderBuckets()
{
	throw std::exception("The method or operation is not implemented.");
}

void RenderBuckets::dispose()
{
	clear();
}

LineBucket* RenderBuckets::addLineBucket( int level, LineStyle *pStyle )
{
	throw std::exception("The method or operation is not implemented.");
}

HairLineBucket* RenderBuckets::addHairLineBucket( int level, LineStyle *pStyle )
{
	throw std::exception("The method or operation is not implemented.");
}

PolygonBucket* RenderBuckets::addPolygonBucket( int level, AreaStyle *pStyle )
{
	throw std::exception("The method or operation is not implemented.");
}

MeshBucket* RenderBuckets::addMeshBucket( int level, AreaStyle *pStyle )
{
	throw std::exception("The method or operation is not implemented.");
}

LineBucket* RenderBuckets::getLineBucket( int level )
{
	throw std::exception("The method or operation is not implemented.");
}

HairLineBucket* RenderBuckets::getHairLineBucket( int level )
{
	throw std::exception("The method or operation is not implemented.");
}

PolygonBucket* RenderBuckets::getPolygonBucket( int level )
{
	throw std::exception("The method or operation is not implemented.");
}

MeshBucket* RenderBuckets::getMeshBucket( int lelve )
{
	throw std::exception("The method or operation is not implemented.");
}

void RenderBuckets::set( RenderBucket *pBuckets )
{
	for (RenderBucket *pIt = m_pBuckets; pIt != NULL; pIt = (RenderBucket *) pIt->next) {
		pIt->clear();
	}
	this->m_pBuckets = pBuckets;
}

RenderBucket* RenderBuckets::get()
{
	return m_pBuckets;
}

RenderBucket* RenderBuckets::getBucket( int level, int type )
{
	throw std::exception("The method or operation is not implemented.");
}

int RenderBuckets::countVboSize()
{
	throw std::exception("The method or operation is not implemented.");
}

int RenderBuckets::countIboSize()
{
	throw std::exception("The method or operation is not implemented.");
}

void RenderBuckets::setFrom( RenderBuckets *pBuckets )
{
	throw std::exception("The method or operation is not implemented.");
}

void RenderBuckets::clear()
{
	throw std::exception("The method or operation is not implemented.");
}

void RenderBuckets::clearBuckets()
{
	throw std::exception("The method or operation is not implemented.");
}

void RenderBuckets::prepare()
{
	throw std::exception("The method or operation is not implemented.");
}

void RenderBuckets::bind()
{
	throw std::exception("The method or operation is not implemented.");
}

bool RenderBuckets::compile( bool addFill )
{
	throw std::exception("The method or operation is not implemented.");
}

void RenderBuckets::initRenderer()
{
	throw std::exception("The method or operation is not implemented.");

// 	LineBucket::Renderer::init();
// 	LineTexBucket::Renderer::init();
// 	PolygonBucket::Renderer::init();
// 	TextureBucket::Renderer::init();
// 	BitmapBucket::Renderer::init();
// 	MeshBucket::Renderer::init();
// 	HairLineBucket::Renderer::init();
}

short RenderBuckets::s = (short) (Tile::SIZE * COORD_SCALE);
short RenderBuckets::fillCoords[8] = {0, s, s, s, 0, 0, s, 0};

