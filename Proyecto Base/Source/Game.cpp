#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Config.h"
#include <SDL.h>

CGame::CGame(){
	estadoJuego = ESTADO_INICIANDO;
	tiempoFrameInicial = CERO;
	tick = CERO;

	atexit(SDL_Quit);
}

void CGame::IniciandoVideo()
{
	if (SDL_Init(SDL_INIT_VIDEO)){
		printf("Error %s ", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	screenBuffer = SDL_SetVideoMode(WIDTH_SCREEN, HEIGHT_SCREEN, 24, SDL_HWSURFACE);
	if (screenBuffer == NULL){
		printf("Error %s ", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_WM_SetCaption("TITULO", NULL);
}

void CGame::CargandoObjetos()
{
	nave = new Nave(screenBuffer, "minave.bmp", (WIDTH_SCREEN / 2), (HEIGHT_SCREEN - 80), MODULO_MINAVE_NAVE);
	menuObjeto = new Objeto(screenBuffer, "Menu.bmp", 0, 0, MODULO_MENU_FONDO);
	textosObjeto = new Objeto(screenBuffer, "Titulos.bmp", 0, 0, -1);
	fondoObjeto = new Objeto(screenBuffer, "Jugando.bmp", 0, 0, 1);
	for (int i = 0; i < 5; i++)
	{
		enemigoArreglo[i] = new Nave(screenBuffer, "enemigo.bmp", i * 60, 0, 2);
		enemigoArreglo[i]->GetNaveObjeto()->SetAutoMovimiento(false);
		enemigoArreglo[i]->GetNaveObjeto()->SetPasoLimite(4);
	}

	opcionSeleccionada = MODULO_TEXTO_MENU_OPCION1;
}
// Con esta función eliminaremos todos los elementos en pantalla
void CGame::Finalize(){
	delete(nave);
	SDL_FreeSurface(screenBuffer);
	SDL_Quit();

}

bool CGame::Start()
{
	// Esta variable nos ayudara a controlar la salida del juego...
	int salirJuego = false;

	while (salirJuego == false){
		//Maquina de estados
		switch (estadoJuego){///ACT2: Mal,, te faltaron 2 estados mas.
		case Estado::ESTADO_INICIANDO:
			IniciandoVideo();
			CargandoObjetos();
			InicializandoStage();
			estadoJuego = Estado::ESTADO_MENU;
			break;
		case Estado::ESTADO_MENU:
			MenuActualizar();
			MenuPintar();
			break;
		case Estado::ESTADO_PRE_JUGANDO:
			nivelActual = CERO;
			vida = UNO;
			enemigosEliminados = CERO;
			estadoJuego = ESTADO_JUGANDO;
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
			estadoJuego = Estado::ESTADO_MENU;
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
		SDL_Flip(screenBuffer);

		//Calculando fps
		tiempoFrameFinal = SDL_GetTicks();
		while (tiempoFrameFinal < (tiempoFrameInicial + FPS_DELAY))
		{
			tiempoFrameFinal = SDL_GetTicks();
			SDL_Delay(1);
		}
#if DEBUG
		printf("Frames:%d Tiempo:%d Tiempo Promedio;%f Tiempo por Fame:%d FPS:%f\n", tick, SDL_GetTicks(), (float)SDL_GetTicks() / (float)tick, tiempoFrameFinal - tiempoFrameInicial, 1000.0f / (float)(tiempoFrameFinal - tiempoFrameInicial));
#endif
		tiempoFrameInicial = tiempoFrameFinal;
		tick++;

	}
	return true;
}

bool CGame::LimitePantalla(Objeto*objeto, int bandera)
{
	if (bandera & BORDE_IZQUIERDO)
		if (objeto->ObtenerX() <= 0)
			return true;
	if (bandera & BORDE_SUPERIOR)
		if (objeto->ObtenerY() <= 0)
			return true;

	if (bandera & BORDE_DERECHO)
		if (objeto->ObtenerX() >= (WIDTH_SCREEN - objeto->ObtenerW()))
			return true;
	if (bandera & BORDE_INFERIOR)
		if (objeto->ObtenerY() >= HEIGHT_SCREEN - objeto->ObtenerH())
			return true;
	return false;

}//Termina LimitePantalla

void CGame::MoverEnemigo(){

	for (int i = 0; i < nivel[nivelActual].NumeroEnemigosVisibles; i++)
	{
		if (enemigoArreglo[i]->GetNaveObjeto()->ObtenerPasoActual() == 0)
			if (!LimitePantalla(enemigoArreglo[i]->GetNaveObjeto(), BORDE_DERECHO))
				enemigoArreglo[i]->GetNaveObjeto()->MoverLados(nivel[nivelActual].VelocidadNaveEnemigo);//Derecha
			else{
				enemigoArreglo[i]->GetNaveObjeto()->IncrementarPasoActual();
			}//fin else derecho

			if (enemigoArreglo[i]->GetNaveObjeto()->ObtenerPasoActual() == 1)
				if (!LimitePantalla(enemigoArreglo[i]->GetNaveObjeto(), BORDE_INFERIOR))
					enemigoArreglo[i]->GetNaveObjeto()->MoverArribaAbajo(nivel[nivelActual].VelocidadNaveEnemigo);//Abajo
				else{
					enemigoArreglo[i]->GetNaveObjeto()->IncrementarPasoActual();
				}//Fn else inferior

				if (enemigoArreglo[i]->GetNaveObjeto()->ObtenerPasoActual() == 2)
					if (!LimitePantalla(enemigoArreglo[i]->GetNaveObjeto(), BORDE_IZQUIERDO))
						enemigoArreglo[i]->GetNaveObjeto()->MoverLados(-nivel[nivelActual].VelocidadNaveEnemigo);//Izquierda
					else{
						enemigoArreglo[i]->GetNaveObjeto()->IncrementarPasoActual();
					}//fin else izquierda

					if (enemigoArreglo[i]->GetNaveObjeto()->ObtenerPasoActual() == 3)
						if (!LimitePantalla(enemigoArreglo[i]->GetNaveObjeto(), BORDE_SUPERIOR))
							enemigoArreglo[i]->GetNaveObjeto()->MoverArribaAbajo(-nivel[nivelActual].VelocidadNaveEnemigo);//Arriba
						else{
							enemigoArreglo[i]->GetNaveObjeto()->IncrementarPasoActual();
						}//fin else arriba
	}
}//Termina MoverEnemigo

void CGame::JugandoPintar(){
	SDL_FillRect(screenBuffer, NULL, SDL_MapRGB(screenBuffer->format, 0, 0, 0));
	fondoObjeto->Pintar();
	////////////////////////////////////////
	//////// CONTROL DE COLISIONES /////////
	for (int i = 0; i < nivel[nivelActual].NumeroEnemigosVisibles; i++)
	{
		if (enemigoArreglo[i]->estaColisionandoConBala(nave))
			vida--;
		if (nave->estaColisionandoConBala(enemigoArreglo[i])){
			enemigoArreglo[i]->setVisible(false);
			enemigosEliminados++;
			nave->simularColision(false);
			if (enemigosEliminados < nivel[nivelActual].NumeroEnemigosEliminar)
			{
				enemigoArreglo[i]->crearNuevo();
			}
		}
	}
	/////////////////////////////////////////
	if (vida <= 0)
		estadoJuego = ESTADO_MENU;

	if (enemigosEliminados >= nivel[nivelActual].NumeroEnemigosEliminar)
	{
		nivelActual++;
	}
	nave->Pintar(NAVE_PROPIA);
	for (int i = 0; i < nivel[nivelActual].NumeroEnemigosVisibles; i++)
	{
		enemigoArreglo[i]->Pintar(NAVE_ENEMIGO);
		enemigoArreglo[i]->AutoDisparar(nivel[nivelActual].velocidadBalasEnemigo);
	}
}

void CGame::JugandoActualizar(){
	keys = SDL_GetKeyState(NULL);

	for (int i = 0; i < nivel[nivelActual].NumeroEnemigosVisibles; i++)
	{
		enemigoArreglo[i]->GetNaveObjeto()->Actualizar();
	}
	MoverEnemigo();

	if (keys[SDLK_UP])
	{
		if (!LimitePantalla(nave->GetNaveObjeto(), BORDE_SUPERIOR))
			nave->MoverArriba(nivel[nivelActual].VelocidadNavePropia);
	}
	if (keys[SDLK_DOWN])
	{
		if (!LimitePantalla(nave->GetNaveObjeto(), BORDE_INFERIOR))
			nave->MoverAbajo(nivel[nivelActual].VelocidadNavePropia);
	}

	if (keys[SDLK_LEFT])
	{
		if (!LimitePantalla(nave->GetNaveObjeto(), BORDE_IZQUIERDO))
			nave->MoverIzquierda(nivel[nivelActual].VelocidadNavePropia);
	}
	if (keys[SDLK_RIGHT])
	{
		if (!LimitePantalla(nave->GetNaveObjeto(), BORDE_DERECHO))
			nave->MoverDerecha(nivel[nivelActual].VelocidadNavePropia);
	}

	if (keys[SDLK_ESCAPE])
	{
		estadoJuego = Estado::ESTADO_MENU;
	}
	if (keys[SDLK_SPACE])
	{
		nave->Disparar(NAVE_PROPIA, nivel[nivelActual].balasMaximas);
	}
	///////////////////////////////////////
	if (keys[SDLK_x]){//bala enemigo / nave nuestra
		nave->simularColision(true);
	}

	if (keys[SDLK_c]){//nuestra bala / nave enemigo
		int enemigoAEliminar = rand() % nivel[nivelActual].NumeroEnemigosVisibles;
		enemigoArreglo[enemigoAEliminar]->simularColision(true);
	}
	if (keys[SDLK_v]){//nuestra nave / nave enemigo

	}
}

void CGame::MenuActualizar()
{
	for (int i = MODULO_TEXTO_MENU_OPCION1, j = 0; i <= MODULO_TEXTO_MENU_OPCION2; i++, j++)
	{
		keys = SDL_GetKeyState(NULL);
		if (keys[SDLK_UP])
		{
			opcionSeleccionada = MODULO_TEXTO_MENU_OPCION1;
		}

		if (keys[SDLK_DOWN])
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
	menuObjeto->Pintar();
	textosObjeto->Pintar(MODULO_TEXTO_TITULO, 150, 0);
	textosObjeto->Pintar(MODULO_TEXTO_NOMBRE, 620, 570);
	textosObjeto->Pintar(MODULO_TEXTO_MENU_OPCION1, 320, 220);
	textosObjeto->Pintar(MODULO_TEXTO_MENU_OPCION2, 320, 250);
}//void	