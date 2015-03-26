#ifndef __OPENGL_IMPLEMENT_H__
#define __OPENGL_IMPLEMENT_H__
#include "Config.h"
#include "SDL_opengl.h"
#include "SDL.h"
#include <stdio.h>
#include <string>
#include <fstream>

class OpenGlImplement
{
	PFNGLCREATEPROGRAMPROC glCreateProgram;
	PFNGLATTACHSHADERPROC glAttachShader;
	PFNGLLINKPROGRAMPROC glLinkProgram;
	PFNGLUSEPROGRAMPROC glUseProgram;
	PFNGLCREATESHADERPROC glCreateShader;
	PFNGLSHADERSOURCEPROC glShaderSource;
	PFNGLCOMPILESHADERPROC glCompileShader;
	PFNGLBINDBUFFERPROC glBindBuffer;
	PFNGLGENBUFFERSPROC glGenBuffers;
	PFNGLBUFFERDATAPROC glBufferData;
	PFNGLDELETEPROGRAMPROC glDeleteProgram;
	PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
	PFNGLGETPROGRAMIVPROC glGetProgramiv;
	PFNGLGETSHADERIVPROC glGetShaderiv;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
	PFNGLISSHADERPROC glIsShader;
	PFNGLISPROGRAMPROC glIsProgram;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

	SDL_Window *window;
	std::string readFile(const char *filePath);
public:
	OpenGlImplement();
	~OpenGlImplement();

	void setSDLWindow(SDL_Window *window);
	void InitGL();
	void InitShaders();
	void InitBuffers(GLuint* gVertexBufferObject, GLuint* gIndexBufferObject, GLfloat* vexterPositions, GLuint vertexDataLen, GLuint* indexData, GLuint indexDataLen);
	void QuitShaders();
	void Draw(GLuint* gVertexBufferObject, GLuint* gIndexBufferObject);
	void DrawStart();
	void DrawEnd();

	GLuint shaderProgram = 0;
	GLint vertexPositionAttribute = -1;
};
#endif