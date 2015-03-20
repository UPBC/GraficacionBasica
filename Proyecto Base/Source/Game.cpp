#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Config.h"
#include <SDL.h>


SDL_bool CGame::CompileShader(GLhandleARB shader, const char *source)
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

SDL_bool CGame::CompileShaderProgram(ShaderData *data)
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



/* Quick utility function for texture creation */
int CGame::power_of_two(int input)
{
	int value = 1;

	while (value < input) {
		value <<= 1;
	}
	return value;
}

SDL_bool CGame::InitShaders()
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

/* A general OpenGL initialization function.    Sets all of the initial parameters. */

void CGame::InitGL(int Width, int Height)                    /* We call this right after our OpenGL window is created. */
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

GLuint CGame::SDL_GL_LoadTexture(SDL_Surface * surface, GLfloat * texcoord)
{
	GLuint texture;
	int w, h;
	SDL_Surface *image;
	SDL_Rect area;
	SDL_BlendMode saved_mode;

	/* Use the surface width and height expanded to powers of 2 */
	w = power_of_two(surface->w);
	h = power_of_two(surface->h);
	texcoord[0] = 0.0f;         /* Min X */
	texcoord[1] = 0.0f;         /* Min Y */
	texcoord[2] = (GLfloat)surface->w / w;     /* Max X */
	texcoord[3] = (GLfloat)surface->h / h;     /* Max Y */

	image = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN     /* OpenGL RGBA masks */
		0x000000FF,
		0x0000FF00, 0x00FF0000, 0xFF000000
#else
		0xFF000000,
		0x00FF0000, 0x0000FF00, 0x000000FF
#endif
		);
	if (image == NULL) {
		return 0;
	}

	/* Save the alpha blending attributes */
	SDL_GetSurfaceBlendMode(surface, &saved_mode);
	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);

	/* Copy the surface into the GL texture image */
	area.x = 0;
	area.y = 0;
	area.w = surface->w;
	area.h = surface->h;
	SDL_BlitSurface(surface, &area, image, &area);

	/* Restore the alpha blending attributes */
	SDL_SetSurfaceBlendMode(surface, saved_mode);

	/* Create an OpenGL texture for the image */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	SDL_FreeSurface(image);     /* No longer needed */

	return texture;
}

/* The main drawing function. */
void CGame::DrawGLScene(SDL_Window *window, GLuint texture, GLfloat * texcoord)
{
	/* Texture coordinate lookup, to make it simple */
	enum {
		MINX,
		MINY,
		MAXX,
		MAXY
	};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        /* Clear The Screen And The Depth Buffer */
	glLoadIdentity();                /* Reset The View */

	glTranslatef(-1.5f, 0.0f, 0.0f);        /* Move Left 1.5 Units */

	/* draw a triangle (in smooth coloring mode) */
	glBegin(GL_POLYGON);                /* start drawing a polygon */
	glColor3f(1.0f, 0.0f, 0.0f);            /* Set The Color To Red */
	glVertex3f(0.0f, 1.0f, 0.0f);        /* Top */
	glColor3f(0.0f, 1.0f, 0.0f);            /* Set The Color To Green */
	glVertex3f(1.0f, -1.0f, 0.0f);        /* Bottom Right */
	glColor3f(0.0f, 0.0f, 1.0f);            /* Set The Color To Blue */
	glVertex3f(-1.0f, -1.0f, 0.0f);        /* Bottom Left */
	glEnd();                    /* we're done with the polygon (smooth color interpolation) */

	glTranslatef(3.0f, 0.0f, 0.0f);         /* Move Right 3 Units */

	/* Enable blending */
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* draw a textured square (quadrilateral) */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0f, 1.0f, 1.0f);
	if (shaders_supported) {
		glUseProgramObjectARB(shaders[current_shader].program);
	}

	glBegin(GL_QUADS);                /* start drawing a polygon (4 sided) */
	glTexCoord2f(texcoord[MINX], texcoord[MINY]);
	glVertex3f(-1.0f, 1.0f, 0.0f);        /* Top Left */
	glTexCoord2f(texcoord[MAXX], texcoord[MINY]);
	glVertex3f(1.0f, 1.0f, 0.0f);        /* Top Right */
	glTexCoord2f(texcoord[MAXX], texcoord[MAXY]);
	glVertex3f(1.0f, -1.0f, 0.0f);        /* Bottom Right */
	glTexCoord2f(texcoord[MINX], texcoord[MAXY]);
	glVertex3f(-1.0f, -1.0f, 0.0f);        /* Bottom Left */
	glEnd();                    /* done with the polygon */

	if (shaders_supported) {
		glUseProgramObjectARB(0);
	}
	glDisable(GL_TEXTURE_2D);

	/* swap buffers to display, since we're double buffered. */
	SDL_GL_SwapWindow(window);
}

void CGame::DestroyShaderProgram(ShaderData *data)
{
	if (shaders_supported) {
		glDeleteObjectARB(data->vert_shader);
		glDeleteObjectARB(data->frag_shader);
		glDeleteObjectARB(data->program);
	}
}

void CGame::QuitShaders()
{
	int i;

	for (i = 0; i < NUM_SHADERS; ++i) {
		DestroyShaderProgram(&shaders[i]);
	}
}

///////////////////////////////////////////////////
///////////////////////////////////////////
//////////////////////////////////////////////////
/////////////////////////////////////////////

CGame::CGame(){
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


	shaders[SHADER_TEXTURE].program = 0;
	shaders[SHADER_TEXTURE].vert_shader = 0;
	shaders[SHADER_TEXTURE].frag_shader = 0;
	shaders[SHADER_TEXTURE].vert_source =
		"varying vec4 v_color;\n"
		"varying vec2 v_texCoord;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
		"    v_color = gl_Color;\n"
		"    v_texCoord = vec2(gl_MultiTexCoord0);\n"
		"}";
	shaders[SHADER_TEXTURE].frag_source =
		"varying vec4 v_color;\n"
		"varying vec2 v_texCoord;\n"
		"uniform sampler2D tex0;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_FragColor = texture2D(tex0, v_texCoord) * v_color;\n"
		"}";


	shaders[SHADER_TEXCOORDS].program = 0;
	shaders[SHADER_TEXCOORDS].vert_shader = 0;
	shaders[SHADER_TEXCOORDS].frag_shader = 0;
	shaders[SHADER_TEXCOORDS].vert_source =
		"varying vec2 v_texCoord;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
		"    v_texCoord = vec2(gl_MultiTexCoord0);\n"
		"}";
	shaders[SHADER_TEXCOORDS].frag_source =
		"varying vec2 v_texCoord;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    vec4 color;\n"
		"    vec2 delta;\n"
		"    float dist;\n"
		"\n"
		"    delta = vec2(0.5, 0.5) - v_texCoord;\n"
		"    dist = dot(delta, delta);\n"
		"\n"
		"    color.r = v_texCoord.x;\n"
		"    color.g = v_texCoord.x * v_texCoord.y;\n"
		"    color.b = v_texCoord.y;\n"
		"    color.a = 1.0 - (dist * 4.0);\n"
		"    gl_FragColor = color;\n"
		"}";

	current_shader = 0;
	estadoJuego = ESTADO_INICIANDO;
	tiempoFrameInicial = CERO;
	tick = CERO;
	atexit(SDL_Quit);
}

void CGame::IniciandoVideo()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	window = SDL_CreateWindow("TITULO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH_SCREEN, HEIGHT_SCREEN, SDL_WINDOW_OPENGL);

	if (!window) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create OpenGL window: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	
	if (!SDL_GL_CreateContext(window)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create OpenGL context: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}

}

void CGame::CargandoObjetos()
{
	nave = new Nave(screenBuffer, "minave.bmp", (WIDTH_SCREEN / 2), (HEIGHT_SCREEN - 80), MODULO_MINAVE_NAVE, NAVE_PROPIA);
	menuFondo = new Objeto(screenBuffer, "Menu.bmp", 0, 0, MODULO_MENU_FONDO);
	textosObjeto = new Objeto(screenBuffer, "Titulos.bmp", 0, 0, -1);
	jugandoFondo = new Objeto(screenBuffer, "Jugando.bmp", 0, 0, 1);
	ganasteFondo = new Objeto(screenBuffer, "Ganaste.bmp", 0, 0, 1);
	perdisteFondo = new Objeto(screenBuffer, "Perdiste.bmp", 0, 0, 1);

	for (int i = 0; i < MAXIMO_DE_ENEMIGOS; i++)
	{
		enemigoArreglo[i] = new Nave(screenBuffer, "enemigo.bmp", i * 2, 0, 2, NAVE_ENEMIGA);
		enemigoArreglo[i]->GetNaveObjeto()->SetAutoMovimiento(false);
		enemigoArreglo[i]->GetNaveObjeto()->SetPasoLimite(4);
	}

	opcionSeleccionada = MODULO_TEXTO_MENU_OPCION1;
}
// Con esta función eliminaremos todos los elementos en pantalla
void CGame::Finalize(){
	delete(nave);
	//SDL_FreeSurface(screenBuffer);
	SDL_Quit();
}

bool CGame::Start()
{
	// Esta variable nos ayudara a controlar la salida del juego...
	int salirJuego = false;

	while (salirJuego == false){
		keys = (Uint8*)SDL_GetKeyboardState(NULL);
		//Maquina de estados
		switch (estadoJuego){///ACT2: Mal,, te faltaron 2 estados mas.
		case Estado::ESTADO_INICIANDO:
			IniciandoVideo();
			CargandoObjetos();
			InicializandoStage();
			////////
			surface = SDL_LoadBMP("enemigo.bmp");
			if (!surface) {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load icon.bmp: %s\n", SDL_GetError());
				SDL_Quit();
				exit(3);
			}

			texture = SDL_GL_LoadTexture(surface, texcoords);
			SDL_FreeSurface(surface);

			/* Loop, drawing and checking events */
			InitGL(WIDTH_SCREEN, HEIGHT_SCREEN);
			if (InitShaders()) {
				SDL_Log("Shaders supported, press SPACE to cycle them.\n");
			}
			else {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Shaders not supported!\n");
			}
			done = 0;

			///////////
			estadoJuego = Estado::ESTADO_MENU;
			break;
		case Estado::ESTADO_MENU:
			while (!done) {
				DrawGLScene(window, texture, texcoords);

				/* This could go in a separate function */
				{ SDL_Event event;
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_QUIT) {
						done = 1;
					}
					if (event.type == SDL_KEYDOWN) {
						if (event.key.keysym.sym == SDLK_SPACE) {
							current_shader = (current_shader + 1) % NUM_SHADERS;
						}
						if (event.key.keysym.sym == SDLK_ESCAPE) {
							done = 1;
						}
					}
				}
				}
			}
			QuitShaders();
			SDL_Quit();
			return 1;
			//MenuActualizar();
			//MenuPintar();
			break;
		case Estado::ESTADO_PRE_JUGANDO:
			nivelActual = CERO;
			vida = UNO;
			enemigosEliminados = CERO;
			estadoJuego = ESTADO_JUGANDO;
			juegoGanado = false;
			IniciarEnemigo();
			IniciarNave();
			break;
		case Estado::ESTADO_JUGANDO:
			JugandoActualizar();
			JugandoPintar();
			break;
		case Estado::ESTADO_FINALIZANDO:
			estadoJuego = Estado::ESTADO_ESPERANDO;
			salirJuego = false;
			break;
		case Estado::ESTADO_TERMINANDO:
			TerminadoPintar();
			TerminadoActualizar();
			break;

		case Estado::ESTADO_ESPERANDO:

			break;
		};

		while (SDL_PollEvent(&event))//Aqui sdl creara una lista de eventos ocurridos
		{
			if (event.type == SDL_QUIT) { salirJuego = true; } //si se detecta una salida del sdl o.....
			if (event.type == SDL_KEYDOWN) {}
		}

		//Este codigo estara provicionalmente aqui.
		//SDL_Flip(screenBuffer);
		

		//Calculando fps
		tiempoFrameFinal = SDL_GetTicks();
		while (tiempoFrameFinal < (tiempoFrameInicial + FPS_DELAY))
		{
			tiempoFrameFinal = SDL_GetTicks();
			SDL_Delay(1);
		}

		tiempoFrameInicial = tiempoFrameFinal;
		tick++;

	}
	return true;
}

bool CGame::LimitePantalla(Objeto*objeto, int bandera)
{
	if (bandera & BORDE_IZQUIERDO)
		if (objeto->GetX() <= 0)
			return true;
	if (bandera & BORDE_SUPERIOR)
		if (objeto->GetY() <= 0)
			return true;

	if (bandera & BORDE_DERECHO)
		if (objeto->GetX() >= (WIDTH_SCREEN - objeto->GetW()))
			return true;
	if (bandera & BORDE_INFERIOR)
		if (objeto->GetY() >= HEIGHT_SCREEN - objeto->GetH())
			return true;
	return false;

}//Termina LimitePantalla

void CGame::MoverEnemigo(){

	for (int i = 0; i < nivel[nivelActual].Enemigos_VisiblesAlMismoTiempo; i++)
	{
		if (enemigoArreglo[i]->GetNaveObjeto()->ObtenerPasoActual() == 0)
			if (!LimitePantalla(enemigoArreglo[i]->GetNaveObjeto(), BORDE_DERECHO))
				enemigoArreglo[i]->GetNaveObjeto()->MoverLados(nivel[nivelActual].Enemigo_Velocidad);//Derecha
			else{
				enemigoArreglo[i]->GetNaveObjeto()->IncrementarPasoActual();
			}//fin else derecho

			if (enemigoArreglo[i]->GetNaveObjeto()->ObtenerPasoActual() == 1)
				if (!LimitePantalla(enemigoArreglo[i]->GetNaveObjeto(), BORDE_INFERIOR))
					enemigoArreglo[i]->GetNaveObjeto()->MoverArribaAbajo(nivel[nivelActual].Enemigo_Velocidad);//Abajo
				else{
					enemigoArreglo[i]->GetNaveObjeto()->IncrementarPasoActual();
				}//Fn else inferior

				if (enemigoArreglo[i]->GetNaveObjeto()->ObtenerPasoActual() == 2)
					if (!LimitePantalla(enemigoArreglo[i]->GetNaveObjeto(), BORDE_IZQUIERDO))
						enemigoArreglo[i]->GetNaveObjeto()->MoverLados(-nivel[nivelActual].Enemigo_Velocidad);//Izquierda
					else{
						enemigoArreglo[i]->GetNaveObjeto()->IncrementarPasoActual();
					}//fin else izquierda

					if (enemigoArreglo[i]->GetNaveObjeto()->ObtenerPasoActual() == 3)
						if (!LimitePantalla(enemigoArreglo[i]->GetNaveObjeto(), BORDE_SUPERIOR))
							enemigoArreglo[i]->GetNaveObjeto()->MoverArribaAbajo(-nivel[nivelActual].Enemigo_Velocidad);//Arriba
						else{
							enemigoArreglo[i]->GetNaveObjeto()->IncrementarPasoActual();
						}//fin else arriba
	}
}//Termina MoverEnemigo

void CGame::JugandoPintar(){
	jugandoFondo->Pintar();
	////////////////////////////////////////
	//////// CONTROL DE COLISIONES /////////
	for (int i = 0; i < nivel[nivelActual].Enemigos_VisiblesAlMismoTiempo; i++)
	{
		
		if (enemigoArreglo[i]->Colision(nave, Nave::TipoColision::NAVE) || enemigoArreglo[i]->Colision(nave, Nave::TipoColision::BALA))//Nave vs Nave Enemigo
			vida--;
		if (nave->Colision(enemigoArreglo[i], Nave::TipoColision::BALA)){//Nave vs Naves Bala
			enemigoArreglo[i]->setVisible(false);
			enemigosEliminados++;
			if (enemigosEliminados < nivel[nivelActual].Enemigo_EliminarPorNivel)
			{
				enemigoArreglo[i]->crearNuevo(rand() % (WIDTH_SCREEN - 64));
			}
		}
	}
	/////////////////////////////////////////
	if (enemigosEliminados >= nivel[nivelActual].Enemigo_EliminarPorNivel)
	{
		if (nivelActual < (MAXIMO_DE_NIVELES-1))
			nivelActual++;
		else{
			juegoGanado = true;
			estadoJuego = ESTADO_TERMINANDO;
		}
	}
	if (vida <= CERO)
		estadoJuego = ESTADO_TERMINANDO;

	nave->Pintar();
	for (int i = 0; i < nivel[nivelActual].Enemigos_VisiblesAlMismoTiempo; i++)
	{
		enemigoArreglo[i]->Pintar();
		enemigoArreglo[i]->AutoDisparar(nivel[nivelActual].Enemigo_VelocidadBala);
	}
}

void CGame::JugandoActualizar(){
	keys = (Uint8 *)SDL_GetKeyboardState(NULL);

	for (int i = 0; i < nivel[nivelActual].Enemigos_VisiblesAlMismoTiempo; i++)
	{
		enemigoArreglo[i]->GetNaveObjeto()->Actualizar();
	}
	MoverEnemigo();

	if (keys[SDLK_UP])
	{
		if (!LimitePantalla(nave->GetNaveObjeto(), BORDE_SUPERIOR))
			nave->MoverArriba(nivel[nivelActual].Nave_Velocidad);
	}
	if (keys[SDLK_DOWN])
	{
		if (!LimitePantalla(nave->GetNaveObjeto(), BORDE_INFERIOR))
			nave->MoverAbajo(nivel[nivelActual].Nave_Velocidad);
	}

	if (keys[SDLK_LEFT])
	{
		if (!LimitePantalla(nave->GetNaveObjeto(), BORDE_IZQUIERDO))
			nave->MoverIzquierda(nivel[nivelActual].Nave_Velocidad);
	}
	if (keys[SDLK_RIGHT])
	{
		if (!LimitePantalla(nave->GetNaveObjeto(), BORDE_DERECHO))
			nave->MoverDerecha(nivel[nivelActual].Nave_Velocidad);
	}

	if (keys[SDLK_ESCAPE])
	{
		estadoJuego = Estado::ESTADO_MENU;
	}
	if (keys[SDLK_SPACE])
	{
		nave->Disparar(nivel[nivelActual].Nave_BalasMaximas);
	}
	
	if (keys[SDLK_c]){//nuestra bala / nave enemigo
		int enemigoAEliminar = rand() % nivel[nivelActual].Enemigos_VisiblesAlMismoTiempo;
		//enemigoArreglo[enemigoAEliminar]->simularColision(true);
	}

	if (keys[SDLK_v]){//nuestra nave / nave enemigo

	}
}

void CGame::MenuActualizar()
{
	for (int i = MODULO_TEXTO_MENU_OPCION1, j = 0; i <= MODULO_TEXTO_MENU_OPCION2; i++, j++)
	{
		if (keys[SDL_SCANCODE_UP])
		{
			opcionSeleccionada = MODULO_TEXTO_MENU_OPCION1;
		}

		if (keys[SDL_SCANCODE_DOWN])
		{
			opcionSeleccionada = MODULO_TEXTO_MENU_OPCION2;
		}

		if (i == opcionSeleccionada)
			textosObjeto->Pintar(i + 2, 320, 220 + (j * 30));
		else
			textosObjeto->Pintar(i, 320, 220 + (j * 30));

		if (keys[SDLK_RETURN])
		{
			if (opcionSeleccionada == MODULO_TEXTO_MENU_OPCION1)
			{
				estadoJuego = Estado::ESTADO_PRE_JUGANDO;
			}

			if (opcionSeleccionada == MODULO_TEXTO_MENU_OPCION2)
			{
				estadoJuego = Estado::ESTADO_FINALIZANDO;
			}
		}// sdlk_return 
	}//for 
}

void CGame::MenuPintar()
{
	menuFondo->Pintar();
	textosObjeto->Pintar(MODULO_TEXTO_TITULO, 150, 0);
	textosObjeto->Pintar(MODULO_TEXTO_NOMBRE, 620, 570);
	textosObjeto->Pintar(MODULO_TEXTO_MENU_OPCION1, 320, 220);
	textosObjeto->Pintar(MODULO_TEXTO_MENU_OPCION2, 320, 250);
}//void	

void CGame::IniciarEnemigo(){
	for (int i = 0; i < nivel[nivelActual].Enemigos_VisiblesAlMismoTiempo; i++)
		enemigoArreglo[i]->crearNuevo(rand() % (WIDTH_SCREEN - 64));
}

void CGame::IniciarNave(){
	nave->crearNuevo(WIDTH_SCREEN / 2);
}

void CGame::TerminadoPintar(){
	if (juegoGanado)
		ganasteFondo->Pintar();
	else
		perdisteFondo->Pintar();
}

void CGame::TerminadoActualizar(){
	if (keys[SDLK_RETURN]){
		estadoJuego = Estado::ESTADO_MENU;
	}
}

