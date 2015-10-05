#include "stdafx.h"
#include "GLState.h"

bool * GLState::vertexArray = new bool[2];
bool GLState::_blend = false;
bool GLState::depth = false;
bool GLState::stencil = false;
int GLState::shader = -1;
float * GLState::clearColor = NULL;
int GLState::glVertexBuffer = -1;
int GLState::glIndexBuffer = -1;
int GLState::currentTexId = -1;

void GLState::init()
{
	vertexArray[0] = false;
	vertexArray[1] = false;

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

bool GLState::useProgram( int shaderProgram )
{
	if (shaderProgram < 0) {
		shader = -1;
	} else if (shaderProgram != shader) {
		glUseProgram(shaderProgram);
		shader = shaderProgram;
		return true;
	}
	return false;
}

void GLState::blend( bool enable )
{
	if (_blend == enable)
		return;

	if (enable)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
	_blend = enable;
}

void GLState::testDepth( bool enable )
{
	if (depth != enable) {
		if (enable)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		depth = enable;
	}
}

void GLState::test( bool depthTest, bool stencilTest )
{
	if (depth != depthTest) {

		if (depthTest)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		depth = depthTest;
	}

	if (stencil != stencilTest) {

		if (stencilTest)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);

		stencil = stencilTest;
	}
}

void GLState::enableVertexArrays( int va1, int va2 )
{
	if (va1 > 1 || va2 > 1) {
		//log.debug("FIXME: enableVertexArrays...");
	}

	if ((va1 == 0 || va2 == 0)) {
		if (!vertexArray[0]) {
			glEnableVertexAttribArray(0);
			vertexArray[0] = true;
		}
	} else {
		if (vertexArray[0]) {
			glDisableVertexAttribArray(0);
			vertexArray[0] = false;
		}
	}

	if ((va1 == 1 || va2 == 1)) {
		if (!vertexArray[1]) {
			glEnableVertexAttribArray(1);
			vertexArray[1] = true;
		}
	} else {
		if (vertexArray[1]) {
			glDisableVertexAttribArray(1);
			vertexArray[1] = false;
		}
	}
}

void GLState::bindTex2D( int id )
{
	if (id < 0) {
		glBindTexture(GL_TEXTURE_2D, 0);
		currentTexId = 0;
	} else if (currentTexId != id) {
		glBindTexture(GL_TEXTURE_2D, id);
		currentTexId = id;
	}
}

void GLState::setClearColor( float* color )
{
	if (clearColor != NULL &&
		color[0] == clearColor[0] &&
		color[1] == clearColor[1] &&
		color[2] == clearColor[2] &&
		color[3] == clearColor[3])
		return;

	clearColor = color;
	glClearColor(color[0], color[1], color[2], color[3]);
}

void GLState::bindBuffer( int target, int id )
{
	if (target == GL_ARRAY_BUFFER) {
		if (glVertexBuffer == id)
			return;
		glVertexBuffer = id;
	}
	else if (target == GL_ELEMENT_ARRAY_BUFFER) {
		if (glIndexBuffer == id)
			return;
		glIndexBuffer = id;
	}
	else {
		//log.debug("invalid target {}", target);
		return;
	}
	//log.debug("bind buffer {} {}", target == GL20.ARRAY_BUFFER, id);

	if (id >= 0)
		glBindBuffer(target, id);
}

void GLState::bindElementBuffer( int id )
{
	if (glIndexBuffer == id)
		return;
	glIndexBuffer = id;

	if (id >= 0)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void GLState::bindVertexBuffer( int id )
{
	if (glVertexBuffer == id)
		return;
	glVertexBuffer = id;

	if (id >= 0)
		glBindBuffer(GL_ARRAY_BUFFER, id);
}
