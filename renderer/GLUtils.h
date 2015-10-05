#pragma once



class GLUtils {

public:
	static void setColor(int location, unsigned int color, float alpha);
	static void setColorBlend(int location, unsigned int color1, unsigned int color2, float mix);
	static void setTextureParameter(int min_filter, int mag_filter, int wrap_s, int wrap_t);
	static int loadTexture(byte* pixel, int width, int height, int format, int min_filter, int mag_filter, int wrap_s, int wrap_t);
	static int loadStippleTexture(byte* stipple, int sizeStipple);
	static void checkGlError(char* op);
	static bool checkGlOutOfMemory(char* op);
	static void setColor(int handle, float* c, float alpha);
	static float* colorToFloat(unsigned int color);
	static float* colorToFloatP(unsigned int color);
	static void changeSaturation(float* color, float change);
	static void glUniform4fv_(int location, int count, float* val);
	static GLuint* glGenBuffers_(int num);
	static void glDeleteBuffers(int num, int* ids);
	static int* glGenTextures(int num);
	static void glDeleteTextures(int num, int* ids);
};