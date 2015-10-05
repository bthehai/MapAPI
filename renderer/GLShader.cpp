#include "stdafx.h"
#include "GLShader.h"
#include "..\backend\GLAdapter.h"
#include "GLUtils.h"
#include "..\backend\AssetAdapter.h"
#include "GLState.h"


bool GLShader::create( std::string vertexSource, std::string fragmentSource )
{
	program = createProgram(vertexSource, fragmentSource);
	return program != 0;
	//throw std::exception("The method or operation is not implemented.");
}

bool GLShader::create( std::string fileName )
{
	program = loadShader(fileName);
	return program != 0;
	//throw std::exception("The method or operation is not implemented.");
}

int GLShader::getAttrib( std::string name )
{
	int loc = glGetAttribLocation(program, name.c_str());
	if (loc < 0) {
		std::cout<<"missing attribute: "<<name<<std::endl;
	}
	return loc;
	//throw std::exception("The method or operation is not implemented.");
}

int GLShader::getUniform( std::string name )
{
	int loc = glGetUniformLocation(program, name.c_str());
	if (loc < 0) {
		std::cout<<"missing uniform: "<<name<<std::endl;
	}
	return loc;
	//throw std::exception("The method or operation is not implemented.");
}

bool GLShader::useProgram()
{
	return GLState::useProgram(program);
	//throw std::exception("The method or operation is not implemented.");
}

int GLShader::loadShader( std::string file )
{
	std::string path = "shaders/" + file + ".glsl";
	std::string vs = AssetAdapter::readTextFile(path);
	//std::string vs = "";
	if (vs == "" ) {
		std::string error = "shader file not found: " + path;
		throw std::exception(error.c_str());
	}
	int fsStart = vs.find('$');
	if (fsStart < 0 || vs.at(fsStart + 1) != '$') {
		std::string error = "not a shader file " + path;
		throw std::exception(error.c_str());
	}

	std::string fs = vs.substr(fsStart + 2);
	vs = vs.substr(0, fsStart);
	int shader = createProgram(vs, fs);
	if (shader == 0) {
		std::cout<<vs<<"\n\n"<<fs<<std::endl;
	}
	return shader;

}

int GLShader::loadShader( int shaderType, std::string source )
{
	GLuint shader = glCreateShader(shaderType);
	if (shader != 0) {
		const char *c_str = source.c_str();
		glShaderSource(shader, 1, &c_str, NULL);
		glCompileShader(shader);

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (compiled == 0) {
			glDeleteShader(shader);
			shader = 0;
		}
	}
	return shader;
}

int GLShader::createProgram( std::string vertexSource, std::string fragmentSource )
{
	std::string defs = "";
	if (GLAdapter::GDX_DESKTOP_QUIRKS) {
		defs += "#define DESKTOP_QUIRKS 1\n";
	} else {
		defs += "#define GLES 1\n";
	}

	int vertexShader = loadShader(GL_VERTEX_SHADER, defs + vertexSource);
	if (vertexShader == 0) {
		return 0;
	}
	int pixelShader = loadShader(GL_FRAGMENT_SHADER, defs + fragmentSource);
	if (pixelShader == 0) {
		return 0;
	}

	int program = glCreateProgram();
	if (program != 0) {
		GLUtils::checkGlError("glCreateProgram");

		glAttachShader(program, vertexShader);
		GLUtils::checkGlError("glAttachShader");

		glAttachShader(program, pixelShader);
		GLUtils::checkGlError("glAttachShader");

		glLinkProgram(program);		
		GLint linkStatus;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_TRUE) {
			glDeleteProgram(program);
			program = 0;
		}
	}

	return program;
}
