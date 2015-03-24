#ifndef __OPENGL_IMPLEMENT_H__
#define __OPENGL_IMPLEMENT_H__
#include "SDL_opengl.h"
#include "SDL.h"


class OpenGlImplement
{
	enum {
		SHADER_COLOR,
		NUM_SHADERS
	};

	typedef struct {
		GLhandleARB program;
		GLhandleARB vert_shader;
		GLhandleARB frag_shader;
		const char *vert_source;
		const char *frag_source;
	} ShaderData;

	PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
	PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
	PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
	PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
	PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
	PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
	PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
	PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
	PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
	PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
	PFNGLUNIFORM1IARBPROC glUniform1iARB;
	PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;

	ShaderData shaders[NUM_SHADERS];
	SDL_Window *window;
	SDL_bool shaders_supported;
	int      current_shader;

public:
	OpenGlImplement();
	void setSDLWindow(SDL_Window *window);
	SDL_Window *getSDLWindow();
	void InitGL(int Width, int Height);
	SDL_bool InitShaders();
	void QuitShaders();
	void DestroyShaderProgram(ShaderData *data);
	SDL_bool CompileShaderProgram(ShaderData *data);
	SDL_bool CompileShader(GLhandleARB shader, const char *source);
	void Draw(SDL_Window *window, GLuint texture, GLfloat * texcoord);
};
#endif