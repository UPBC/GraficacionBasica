#include "OpegGLImplement.h"

OpenGlImplement::OpenGlImplement(){
	shaders[SHADER_COLOR].program = 0;
	shaders[SHADER_COLOR].vert_shader = 0;
	shaders[SHADER_COLOR].frag_shader = 0;
	shaders[SHADER_COLOR].vert_source =
		"varying vec4 v_color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
		"    v_color = gl_Color;\n"
		"}";
	shaders[SHADER_COLOR].frag_source =
		"varying vec4 v_color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_FragColor = v_color;\n"
		"}";

	current_shader = 0;
}

void OpenGlImplement::setSDLWindow(SDL_Window *window){
	this->window = window;
}

void OpenGlImplement::InitGL(int Width, int Height)                    /* We call this right after our OpenGL window is created. */
{
	GLdouble aspect;

	glViewport(0, 0, Width, Height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);        /* This Will Clear The Background Color To Black */
	glClearDepth(1.0);                /* Enables Clearing Of The Depth Buffer */
	glDepthFunc(GL_LESS);                /* The Type Of Depth Test To Do */
	glEnable(GL_DEPTH_TEST);            /* Enables Depth Testing */
	glShadeModel(GL_SMOOTH);            /* Enables Smooth Color Shading */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();                /* Reset The Projection Matrix */

	aspect = (GLdouble)Width / Height;
	glOrtho(-3.0, 3.0, -3.0 / aspect, 3.0 / aspect, 0.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
}

SDL_bool OpenGlImplement::InitShaders()
{
	int i;

	/* Check for shader support */
	shaders_supported = SDL_FALSE;
	if (SDL_GL_ExtensionSupported("GL_ARB_shader_objects") &&
		SDL_GL_ExtensionSupported("GL_ARB_shading_language_100") &&
		SDL_GL_ExtensionSupported("GL_ARB_vertex_shader") &&
		SDL_GL_ExtensionSupported("GL_ARB_fragment_shader")) {
		glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)SDL_GL_GetProcAddress("glAttachObjectARB");
		glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)SDL_GL_GetProcAddress("glCompileShaderARB");
		glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)SDL_GL_GetProcAddress("glCreateProgramObjectARB");
		glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)SDL_GL_GetProcAddress("glCreateShaderObjectARB");
		glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)SDL_GL_GetProcAddress("glDeleteObjectARB");
		glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)SDL_GL_GetProcAddress("glGetInfoLogARB");
		glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)SDL_GL_GetProcAddress("glGetObjectParameterivARB");
		glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)SDL_GL_GetProcAddress("glGetUniformLocationARB");
		glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)SDL_GL_GetProcAddress("glLinkProgramARB");
		glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)SDL_GL_GetProcAddress("glShaderSourceARB");
		glUniform1iARB = (PFNGLUNIFORM1IARBPROC)SDL_GL_GetProcAddress("glUniform1iARB");
		glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)SDL_GL_GetProcAddress("glUseProgramObjectARB");
		if (glAttachObjectARB &&
			glCompileShaderARB &&
			glCreateProgramObjectARB &&
			glCreateShaderObjectARB &&
			glDeleteObjectARB &&
			glGetInfoLogARB &&
			glGetObjectParameterivARB &&
			glGetUniformLocationARB &&
			glLinkProgramARB &&
			glShaderSourceARB &&
			glUniform1iARB &&
			glUseProgramObjectARB) {
			shaders_supported = SDL_TRUE;
		}
	}

	if (!shaders_supported) {
		return SDL_FALSE;
	}

	/* Compile all the shaders */
	for (i = 0; i < NUM_SHADERS; ++i) {
		if (!CompileShaderProgram(&shaders[i])) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to compile shader!\n");
			return SDL_FALSE;
		}
	}

	/* We're done! */
	return SDL_TRUE;
}

SDL_bool OpenGlImplement::CompileShader(GLhandleARB shader, const char *source)
{
	GLint status;

	glShaderSourceARB(shader, 1, &source, NULL);
	glCompileShaderARB(shader);
	glGetObjectParameterivARB(shader, GL_OBJECT_COMPILE_STATUS_ARB, &status);
	if (status == 0) {
		GLint length;
		char *info;

		glGetObjectParameterivARB(shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
		info = SDL_stack_alloc(char, length + 1);
		glGetInfoLogARB(shader, length, NULL, info);
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to compile shader:\n%s\n%s", source, info);
		SDL_stack_free(info);

		return SDL_FALSE;
	}
	else {
		return SDL_TRUE;
	}
}

SDL_bool OpenGlImplement::CompileShaderProgram(ShaderData *data)
{
	const int num_tmus_bound = 4;
	int i;
	GLint location;

	glGetError();

	/* Create one program object to rule them all */
	data->program = glCreateProgramObjectARB();

	/* Create the vertex shader */
	data->vert_shader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	if (!CompileShader(data->vert_shader, data->vert_source)) {
		return SDL_FALSE;
	}

	/* Create the fragment shader */
	data->frag_shader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	if (!CompileShader(data->frag_shader, data->frag_source)) {
		return SDL_FALSE;
	}

	/* ... and in the darkness bind them */
	glAttachObjectARB(data->program, data->vert_shader);
	glAttachObjectARB(data->program, data->frag_shader);
	glLinkProgramARB(data->program);

	/* Set up some uniform variables */
	glUseProgramObjectARB(data->program);
	for (i = 0; i < num_tmus_bound; ++i) {
		char tex_name[5];
		SDL_snprintf(tex_name, SDL_arraysize(tex_name), "tex%d", i);
		location = glGetUniformLocationARB(data->program, tex_name);
		if (location >= 0) {
			glUniform1iARB(location, i);
		}
	}
	glUseProgramObjectARB(0);

	return (glGetError() == GL_NO_ERROR) ? SDL_TRUE : SDL_FALSE;
}

void OpenGlImplement::DestroyShaderProgram(ShaderData *data)
{
	if (shaders_supported) {
		glDeleteObjectARB(data->vert_shader);
		glDeleteObjectARB(data->frag_shader);
		glDeleteObjectARB(data->program);
	}
}

void OpenGlImplement::QuitShaders()
{
	int i;

	for (i = 0; i < NUM_SHADERS; ++i) {
		DestroyShaderProgram(&shaders[i]);
	}
}
