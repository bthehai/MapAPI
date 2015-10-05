#pragma once

class GLState {

private:
	static bool *vertexArray;
	static bool _blend;
	static bool depth;
	static bool stencil;
	static int shader;
	static float *clearColor;
	static int glVertexBuffer;
	static int glIndexBuffer;
	static int currentTexId;

public:
	static void init();
	static bool useProgram(int shaderProgram);
	static void blend(bool enable);
	static void testDepth(bool enable);
	static void test(bool depthTest, bool stencilTest);
	static void enableVertexArrays(int va1, int va2);
	static void bindTex2D(int id);
	static void setClearColor(float* color);
	static void bindBuffer(int target, int id);
	static void bindElementBuffer(int id);
	static void bindVertexBuffer(int id);

};