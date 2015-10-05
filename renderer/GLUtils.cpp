#include "stdafx.h"
#include "GLUtils.h"
#include <math.h>
#include "..\utils\FastMath.h"
#include "GLState.h"


#define GDX_WEBGL_QUIRKS false

void GLUtils::setColor(int location, unsigned int color, float alpha) {
	if (alpha >= 1)
		alpha = ((color >> 24) & 0xff) / 255.0f;
	else if (alpha < 0)
		alpha = 0;
	else
		alpha *= ((color >> 24) & 0xff) / 255.0f;

	if (alpha == 1) {
		glUniform4f(location,
		             ((color >> 16) & 0xff) / 255.0f,
		             ((color >> 8) & 0xff) / 255.0f,
		             ((color >> 0) & 0xff) / 255.0f,
		             alpha);
	} else {
		glUniform4f(location,
		             ((color >> 16) & 0xff) / 255.0f * alpha,
		             ((color >> 8) & 0xff) / 255.0f * alpha,
		             ((color >> 0) & 0xff) / 255.0f * alpha,
		             alpha);
	}
}

void GLUtils::setColorBlend(int location, unsigned int color1, unsigned int color2, float mix) {
	float a1 = (((color1 >> 24) & 0xff) / 255.0f) * (1 - mix);
	float a2 = (((color2 >> 24) & 0xff) / 255.0f) * mix;
	glUniform4f(location,
	             ((((color1 >> 16) & 0xff) / 255.0f) * a1
	             + (((color2 >> 16) & 0xff) / 255.0f) * a2),
	             ((((color1 >> 8) & 0xff) / 255.0f) * a1
	             + (((color2 >> 8) & 0xff) / 255.0f) * a2),
	             ((((color1 >> 0) & 0xff) / 255.0f) * a1
	             + (((color2 >> 0) & 0xff) / 255.0f) * a2),
	             (a1 + a2));
}

void GLUtils::setTextureParameter(int min_filter, int mag_filter, int wrap_s, int wrap_t) {
	glTexParameterf(GL_TEXTURE_2D,
	                 GL_TEXTURE_MIN_FILTER,
	                 (GLfloat) min_filter);
	glTexParameterf(GL_TEXTURE_2D,
	                 GL_TEXTURE_MAG_FILTER,
	                 (GLfloat) mag_filter);
	glTexParameterf(GL_TEXTURE_2D,
	                 GL_TEXTURE_WRAP_S,
	                 (GLfloat) wrap_s); // Set U Wrapping
	glTexParameterf(GL_TEXTURE_2D,
	                 GL_TEXTURE_WRAP_T,
	                 (GLfloat) wrap_t); // Set V Wrapping
}

int GLUtils::loadTexture(byte* pixel, int width, int height, int format, int min_filter, int mag_filter, int wrap_s, int wrap_t) {

	int* textureIds = GLUtils::glGenTextures(1);
	GLState::bindTex2D(textureIds[0]);

	setTextureParameter(min_filter, mag_filter, wrap_s, wrap_t);

// 	ByteBuffer buf = ByteBuffer.allocateDirect(width * height).order(ByteOrder.nativeOrder());
// 	buf.put(pixel);
// 	buf.position(0);
// 	IntBuffer intBuf = buf.asIntBuffer();
// 
// 	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
// 	              GL_UNSIGNED_BYTE, intBuf);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixel);

	GLState::bindTex2D(0);

	return textureIds[0];
}

int GLUtils::loadStippleTexture(byte* stipple, int sizeStipple) {
	int sum = 0;
	int i;
	for (i = 0; i < sizeStipple; i++) 
		sum += stipple[i];	

	byte* pixel = new byte[sum];

	bool on = true;
	int pos = 0;
	for (i = 0; i < sizeStipple; i++) {
		float max = stipple[i];

		for (int s = 0; s < stipple[i]; s++) {
			float color = abs(s / (max - 1) - 0.5f);
			if (on)
				color = 255 * (1 - color);
			else
				color = 255 * color;

			pixel[pos + s] = clampToByte((int) color);
		}
		on = !on;
		pos += stipple[i];
	}

	return loadTexture(pixel, sum, 1, GL_ALPHA,
	                   GL_LINEAR, GL_LINEAR,
	                   // GLES20.GL_NEAREST, GLES20.GL_NEAREST,
	                   GL_REPEAT,
	                   GL_REPEAT);
}

void GLUtils::checkGlError(char* op) {
	//GL = GLAdapter.get();

	int error;
	while ((error = glGetError()) != 0) { // GL20.NO_ERROR) {
		std::cout<<op<<": glError"<<error<<std::endl;
		//log.error(op + ": glError " + error);
		// throw new RuntimeException(op + ": glError " + error);
	}
}

bool GLUtils::checkGlOutOfMemory(char* op) {
	int error;
	bool oom = false;
	while ((error = glGetError()) != 0) {// GL20.NO_ERROR) {
		std::cout<<op<<": glError"<<error<<std::endl;
		//log.error(op + ": glError " + error);
		// throw new RuntimeException(op + ": glError " + error);
		if (error == 1285)
			oom = true;
	}
	return oom;
}

void GLUtils::setColor(int handle, float* c, float alpha) {
	if (alpha >= 1) {
		glUniform4f(handle, c[0], c[1], c[2], c[3]);
	} else {
		if (alpha < 0) {
			//log.debug("setColor: " + alpha);
			alpha = 0;
			glUniform4f(handle, 0, 0, 0, 0);
		}

		glUniform4f(handle,
		             c[0] * alpha, c[1] * alpha,
		             c[2] * alpha, c[3] * alpha);
	}
}

float* GLUtils::colorToFloat(unsigned int color) {
	float* c = new float[4];
	c[3] = (color >> 24 & 0xff) / 255.0f;
	c[0] = (color >> 16 & 0xff) / 255.0f;
	c[1] = (color >> 8 & 0xff) / 255.0f;
	c[2] = (color >> 0 & 0xff) / 255.0f;
	return c;
}

// premultiply alpha
float* GLUtils::colorToFloatP(unsigned int color) {
	float* c = new float[4];
	c[3] = (color >> 24 & 0xff) / 255.0f;
	c[0] = (color >> 16 & 0xff) / 255.0f * c[3];
	c[1] = (color >> 8 & 0xff) / 255.0f * c[3];
	c[2] = (color >> 0 & 0xff) / 255.0f * c[3];
	return c;
}

/**
 * public-domain function by Darel Rex Finley from
 * http://alienryderflex.com/saturation.html
 * 
 * @param color
 *            The passed-in RGB values can be on any desired scale, such as
 *            0 to 1, or 0 to 255.
 * @param change
 *            0.0 creates a black-and-white image. 0.5 reduces the color
 *            saturation by half. 1.0 causes no change. 2.0 doubles the
 *            color saturation.
 */
void GLUtils::changeSaturation(float* color, float change) {
	float r = color[0];
	float g = color[1];
	float b = color[2];
	double p = sqrt(r * r * 0.299f + g * g * 0.587f + b * b * 0.114f);
	color[0] = clampN((float) (p + (r - p) * change));
	color[1] = clampN((float) (p + (g - p) * change));
	color[2] = clampN((float) (p + (b - p) * change));
}

void GLUtils::glUniform4fv_(int location, int count, float* val) {
// 	FloatBuffer buf = MapRenderer::getFloatBuffer(count * 4);
// 	buf.put(val);
// 	buf.flip();
// 	glUniform4fv(location, count, buf);
	glUniform4fv(location, count, val);
}

GLuint* GLUtils::glGenBuffers_(int num) {
// 	IntBuffer buf = MapRenderer::getIntBuffer(num);
// 	buf.position(0);
// 	buf.limit(num);
// 	glGenBuffers(num, buf);
// 	int[] ret = new int[num];
// 	buf.position(0);
// 	buf.limit(num);
// 	buf.get(ret);
// 	return ret;
	GLuint *buf = new GLuint[num];
	glGenBuffers(num, buf);
	return buf;
}

void GLUtils::glDeleteBuffers(int num, int* ids) {
// 	IntBuffer buf = MapRenderer::getIntBuffer(num);
// 	buf.put(ids, 0, num);
// 	buf.flip();
// 	glDeleteBuffers(num, buf);
}

int* GLUtils::glGenTextures(int num) {
	if (num <= 0)
		return NULL;

	int* ret = new int[num];
// 	IntBuffer buf = MapRenderer.getIntBuffer(num);
// 
// 	if (GDX_WEBGL_QUIRKS) {
// 		for (int i = 0; i < num; i++) {
// 			glGenTextures(num, buf);
// 			buf.position(0);
// 			ret[i] = buf.get();
// 			buf.position(0);
// 		}
// 	} else {
// 		glGenTextures(num, buf);
// 		buf.position(0);
// 		buf.get(ret);
// 	}

	return ret;
}

void GLUtils::glDeleteTextures(int num, int* ids) {
// 	IntBuffer buf = MapRenderer::getIntBuffer(num);
// 	buf.put(ids, 0, num);
// 	buf.flip();
// 	glDeleteTextures(num, buf);
}