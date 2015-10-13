#pragma once
#include "TileRenderer.h"

class GLMatrix;

class VectorTileRenderer : public TileRenderer {

public:
	VectorTileRenderer();
	virtual ~VectorTileRenderer();

protected:
	int m_nClipMode;
	int m_nDrawSerial;

	GLMatrix *m_pClipProj;
	GLMatrix *m_pClipMVP;

public:
	virtual void render(GLViewport *pViewport);

private:
	void drawTile(MapTile *pTile, GLViewport *pViewport, int proxyLevel);

protected:
	bool drawChildren(MapTile *pTile, GLViewport *pViewport);
	bool drawParent(MapTile *pTile, GLViewport *pViewport);
	void drawGrandParent(MapTile *pTile, GLViewport *pViewport);

};