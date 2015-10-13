#include "stdafx.h"
#include "MapRenderer.h"
#include "BufferObject.h"
#include "GLUtils.h"
#include "bucket\TextureItem.h"
#include "GLState.h"
#include "..\backend\GLAdapter.h"
#include "..\map\Animator.h"
#include "..\map\Layers.h"
#include "LayerRenderer.h"
#include "bucket\RenderBuckets.h"


#define COLOR_DKGRAY 0xffa9a9a9


float * MapRenderer::m_ClearColor = NULL;
bool MapRenderer::m_bRerender = true;
int MapRenderer::m_nQuadVerticesID = 0;
int MapRenderer::m_nQuadIndicesID = 0;
NativeBufferPool* MapRenderer::m_pBufferPool = NULL;

long MapRenderer::frameTime = 0;


MapRenderer::MapRenderer( Map *pMap )
	: m_pMap(pMap)
{
	m_pViewPort = new GLViewport();
	if (m_pBufferPool != NULL) {
		delete m_pBufferPool;
		m_pBufferPool = NULL;
	}
	m_pBufferPool = new NativeBufferPool();

	BufferObject::clear();
	setBackgroundColor(COLOR_DKGRAY);

	m_bNewSurface = true;
}

MapRenderer::~MapRenderer()
{
	if (m_pViewPort != NULL) {
		delete m_pViewPort;
		m_pViewPort = NULL;
	}
	if (m_pBufferPool != NULL) {
		delete m_pBufferPool;
		m_pBufferPool = NULL;
	}
	if (m_ClearColor != NULL) {
		delete m_ClearColor;
		m_ClearColor = NULL;
	}
}

void MapRenderer::setBackgroundColor( int color )
{
	if (m_ClearColor != NULL) {
		delete m_ClearColor;
		m_ClearColor = NULL;
	}
	m_ClearColor = GLUtils::colorToFloat(color);
}

void MapRenderer::onDrawFrame()
{
	//frameTime = GetTickCount();
	draw();
	m_pBufferPool->releaseBuffers();

	TextureItem::disposeTextures();
}

void MapRenderer::draw()
{
	GLState::setClearColor(m_ClearColor);
	
	glDepthMask(true);
	glStencilMask(0xFF);

	glClear(GL_COLOR_BUFFER_BIT 
		| GL_DEPTH_BUFFER_BIT
		| GL_STENCIL_BUFFER_BIT);

	glDepthMask(false);
	glStencilMask(0);

	GLState::test(false, false);
	GLState::blend(false);
	GLState::bindTex2D(-1);
	GLState::useProgram(-1);
	GLState::bindElementBuffer(-1);
	GLState::bindVertexBuffer(-1);

	Animator *pAnimator = m_pMap->animator();
	pAnimator->updateAnimation();
	GLViewport *pViewport = (GLViewport *)m_pMap->viewport();
	m_pViewPort->setFrom(pViewport);

	if (DEBUG) {
		m_pViewPort->mvp.setScale(0.5f, 0.5f, 1);
		m_pViewPort->viewproj.multiplyLhs(m_pViewPort->mvp);
		m_pViewPort->proj.multiplyLhs(m_pViewPort->mvp);
	}

	Layers *pLayers = m_pMap->layers();
	LayerRenderer **pLayerRenderers = pLayers->getLayerRenderer();
	int nLayerRenderers = pLayers->rendererSize();
	for (int i = 0; i < nLayerRenderers; i++) {
		LayerRenderer* pRenderer = pLayerRenderers[i];
		if (!pRenderer->isInitialized) {
			pRenderer->setup();
			pRenderer->isInitialized = true;
		}

		pRenderer->update(m_pViewPort);

		if (pRenderer->isReady) {
			pRenderer->render(m_pViewPort);
		}

		if (DEBUG) {
			GLUtils::checkGlError("Renderer");
		}
	}

	if (GLUtils::checkGlOutOfMemory("Finish")) {
		BufferObject::checkBufferUsage(true);
	}

	if (m_bRerender) {
		m_pMap->render();
		m_bRerender = false;
	}
}

void MapRenderer::onSurfaceChanged( int width, int height )
{
	if (width <= 0 || height <= 0) {
		return;
	}

	m_pViewPort->initFrom(m_pMap->viewport());

	glViewport(0, 0, width, height);

	glClearStencil(0x00);

	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	if (!m_bNewSurface) {
		m_pMap->updateMap(false);
		return;
	}
	m_bNewSurface = false;
	
	GLuint* vboIds = GLUtils::glGenBuffers_(2);
	m_nQuadIndicesID = vboIds[0];	

	short indices[MAX_INDICES];
	for (int i = 0, j = 0; i < MAX_INDICES; i += 6, j += 4) {
		indices[i + 0] = (short) (j + 0);
		indices[i + 1] = (short) (j + 1);
		indices[i + 2] = (short) (j + 2);

		indices[i + 3] = (short) (j + 2);
		indices[i + 4] = (short) (j + 1);
		indices[i + 5] = (short) (j + 3);
	}

	GLState::bindElementBuffer(m_nQuadIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_INDICES * sizeof(short), indices, GL_STATIC_DRAW);
	GLState::bindElementBuffer(0);

	/** initialize default quad */
	float quad[8] = {-1, -1, -1, 1, 1, -1, 1, 1};
	m_nQuadVerticesID = vboIds[1];
	GLState::bindVertexBuffer(m_nQuadVerticesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * sizeof(float), quad, GL_STATIC_DRAW);
	GLState::bindVertexBuffer(0);

	GLState::init();
	m_pMap->updateMap(true);

	delete []vboIds;
}

void MapRenderer::onSurfaceCreate()
{
	 const GLubyte* vendor = glGetString(GL_VENDOR);
	 const GLubyte* renderer = glGetString(GL_RENDERER);
	 const GLubyte* version = glGetString(GL_VERSION);

	 std::cout<<vendor<<"|"<<renderer<<"|"<<version<<std::endl;
	 
	 GLState::init();

	 BufferObject::init(200);

	 RenderBuckets::initRenderer();

	 m_bNewSurface = true;
}

FloatMBuffer* MapRenderer::getFloatBuffer( int size )
{
	throw std::exception("The method or operation is not implemented.");
}

ShortMBuffer* MapRenderer::getShortBuffer( int size )
{
	throw std::exception("The method or operation is not implemented.");
}

IntMBuffer* MapRenderer::getIntBuffer( int size )
{
	throw std::exception("The method or operation is not implemented.");
}

void MapRenderer::bindQuadVertexVBO( int location )
{
	if (location > 0) {
		GLState::bindVertexBuffer(m_nQuadVerticesID);
		GLState::enableVertexArrays(location, -1);
		glVertexAttribPointer(location, 2, GL_FLOAT, false, 0, 0); 
	}
}

void MapRenderer::bindQuadIndicesVBO()
{
	GLState::bindElementBuffer(m_nQuadIndicesID);
}

void MapRenderer::animate()
{
	m_bRerender = true;
}


