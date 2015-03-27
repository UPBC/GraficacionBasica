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
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	PFNGLGETPROGRAMIVPROC glGetProgramiv;
	PFNGLGETSHADERIVPROC glGetShaderiv;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
	PFNGLISSHADERPROC glIsShader;
	PFNGLISPROGRAMPROC glIsProgram;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	PFNGLACTIVETEXTUREPROC glActiveTexture;
	PFNGLUNIFORM1IPROC glUniform1i;

	SDL_Window *window;
	std::string readFile(const char *filePath);
public:
	OpenGlImplement();
	~OpenGlImplement();

	void setSDLWindow(SDL_Window *window);
	void InitGL();
	void InitShaders();
	void InitBuffers(GLuint* vertexBufferObject, GLuint* indexBufferObject, GLuint* textureBufferObject, GLfloat* vexterPositions, GLuint vertexDataLen, GLuint* indexData, GLuint indexDataLen, GLfloat* textureData, GLuint textureDataLen);
	void QuitShaders();
	void Draw(GLuint* vertexBufferObject, GLuint* indexBufferObject, GLuint* textureBufferObject);
	void DrawStart();
	void DrawEnd();
	GLuint LoadTexture(SDL_Surface * surface);

	GLuint shaderProgram = 0;
	GLint vertexPositionAttribute = -1;
	GLint vertexTextureCoordAttribute = -1;
	GLint samplerUniform = -1;

};
#endif