#pragma once
//#include <SDL_image.h>
#include "Objeto.h"
#include "Nave.h"
#include "Stage.h"
#include "SDL_opengl.h"



class CGame
{
public:
	bool Start();
	static CGame instanceGame;

	CGame();
	void Finalize();
	
	enum Estado{
		ESTADO_INICIANDO,
		ESTADO_MENU,
		ESTADO_PRE_JUGANDO,
		ESTADO_JUGANDO,
		ESTADO_TERMINANDO,
		ESTADO_FINALIZANDO,
		ESTADO_ESPERANDO
	};


private:
	///// NUEVO
	enum {
		SHADER_COLOR,
		SHADER_TEXTURE,
		SHADER_TEXCOORDS,
		NUM_SHADERS
	};

	typedef struct {
		GLhandleARB program;
		GLhandleARB vert_shader;
		GLhandleARB frag_shader;
		const char *vert_source;
		const char *frag_source;
	} ShaderData;

	SDL_bool shaders_supported;
	int      current_shader;

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

	////////////
	void CargandoObjetos();
	void IniciandoVideo();
	void MoverEnemigo();
	void MenuPintar();
	void MenuActualizar();
	void JugandoPintar();
	void JugandoActualizar();
	void TerminadoPintar();
	void TerminadoActualizar();
	bool LimitePantalla(Objeto * objeto, int bandera);
	void InicializandoStage();
	void IniciarEnemigo();
	void IniciarNave();
	
	Uint8 *keys;//Esta variable nos servira para ver si determinadas teclas estan o no pulsadas
	SDL_Event event;//La variable event de tipo evento de SDL nos servira para motorizar
	SDL_Surface *screenBuffer;
	
	///////// NUEVO
	SDL_Window *window;
	
	SDL_Surface *surface;
	GLuint texture;
	GLfloat texcoords[4];
	int done;

	void QuitShaders();
	void DestroyShaderProgram(ShaderData *data);
	void DrawGLScene(SDL_Window *window, GLuint texture, GLfloat * texcoord);
	GLuint SDL_GL_LoadTexture(SDL_Surface * surface, GLfloat * texcoord);
	void InitGL(int Width, int Height);
	SDL_bool InitShaders();
	int power_of_two(int input);
	SDL_bool CompileShaderProgram(ShaderData *data);
	SDL_bool CompileShader(GLhandleARB shader, const char *source);
	///////////

	int opcionSeleccionada;
	int nivelActual;
	int vida;
	int enemigosEliminados;
	bool juegoGanado;

	Nave *nave;
	Nave *enemigoArreglo[MAXIMO_DE_ENEMIGOS];
	Stage nivel[MAXIMO_DE_NIVELES];
	
	Estado estadoJuego;
	
	int tick;
	int tiempoFrameInicial;
	int tiempoFrameFinal;

	Objeto *menuFondo;//Fondo del menu
	Objeto *textosObjeto;//Textos del juego
	Objeto *jugandoFondo;//Fondo del juego
	Objeto *ganasteFondo;//Fondo cuando ganas
	Objeto *perdisteFondo;//Fondo cuando ganas

};