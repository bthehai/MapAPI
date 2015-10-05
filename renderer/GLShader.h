#pragma once
#include <string>

class GLShader {
public:
	int program;

protected:
	bool create(std::string vertexSource, std::string fragmentSource);
	bool create(std::string fileName);

	int getAttrib(std::string name);
	int getUniform(std::string name);

	virtual bool useProgram();

public:
	static int loadShader(std::string file);
	static int loadShader(int shaderType, std::string source);
	static int createProgram(std::string vertexSource, std::string fragmentSource);


};