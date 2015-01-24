#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Config.h"
#include <SDL.h>
#include <SDL_image.h>

CGame::CGame(){
	estado = ESTADO_INICIANDO;
	condicion = ESTADO_TERMINANDO;//ACT2:Mal, Aqui debes de iniciar tu estado, revisa el diagrama para saber cual es el estado inicial
atexit(SDL_Quit);
tiempoFrameInicial = 0;
}

void CGame::Iniciando()
{
	if (SDL_Init(SDL_INIT_VIDEO)){
		printf("Error %s ", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	screen = SDL_SetVideoMode(WIDTH_SCREEN, HEIGHT_SCREEN, 24, SDL_HWSURFACE);
	if (screen == NULL){
		printf("Error %s ", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_WM_SetCaption("Mi primer Juego", NULL);
	atexit(SDL_Quit);
	//x = (WIDTH_SCREEN/2)-(sprite->WidthModule(0)/2);//Ancho
	//	y = (HEIGHT_SCREEN-80)-(sprite->HeightModule(0));//Alto
	nave = new Nave(screen, "../Data/MiNave.bmp", (WIDTH_SCREEN / 2)/*-(sprite->WidthModule(0)/2)*/, (HEIGHT_SCREEN - 80)/*-(sprite->HeightModule(0))*/, MODULO_MINAVE_NAVE);
	menu = new Objeto(screen, "../Data/Menu.bmp", 0, 0, MODULO_MENU_FONDO);
	textos = new Objeto(screen, "../Data/Titulos.bmp", 0, 0, -1);
	fondo = new Objeto(screen, "../Data/Jugando.bmp", 0, 0, 1);
for (int i = 0; i < 5; i++)
{
	enemigoArreglo[i] = new Nave(screen, "../Data/enemigo.bmp", i * 60, 0, 2);
	enemigoArreglo[i]->GetNaveObjeto()->SetAutoMovimiento(false);
	enemigoArreglo[i]->GetNaveObjeto()->SetPasoLimite(4);
}

tick = 0;
opcionSeleccionada = MODULO_TEXTO_MENU_OPCION1;


//nave = new Sprite(screen);
//nave->cargarimagen("../Data/MiNave.bmp");
}
// Con esta función eliminaremos todos los elementos en pantalla
void CGame::Finalize(){
	delete(nave);
	//delete(keys);
	//delete(enemigo);
	//delete(enemigoArreglo);
	SDL_FreeSurface(screen);
	SDL_Quit();
}

bool CGame::Start()
{
	// Esta variable nos ayudara a controlar la salida del juego...
	int salirJuego = false;
          
	while (salirJuego == false){
		//Maquina de estados
		switch (estado){///ACT2: Mal,, te faltaron 2 estados mas.
		case Estado::ESTADO_INICIANDO:
			Iniciando();
			Inicializandotage();
			estado = Estado::ESTADO_MENU;
			break;
		case Estado::ESTADO_MENU:
			menu->Pintar();
			textos->Pintar(MODULO_TEXTO_TITULO,150,0);
			textos->Pintar(MODULO_TEXTO_NOMBRE, 620, 570);
			textos->Pintar(MODULO_TEXTO_MENU_OPCION1, 320, 220);
			textos->Pintar(MODULO_TEXTO_MENU_OPCION2, 320, 250);
			Menu();
			/*textos->Pintar(MODULO_TEXTO_TITULO, 3, 0);
			textos->Pintar(MODULO_TEXTO_TITULO, 3, 0);*/
			//estado = Estado::ESTADO_JUGANDO;
			break;
		case Estado::ESTADO_PRE_JUGANDO:
			nivelActual = 0;
			vida = 1;
			enemigosEliminados = 0;
			estado = ESTADO_JUGANDO;
				break;

		case Estado::ESTADO_JUGANDO:
			for (int i = 0; i < nivel[nivelActual].NumeroEnemigosVisibles; i++)
			{
				enemigoArreglo[i]->GetNaveObjeto()->Actualizar();
			}
			MoverEnemigo();
			SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0
				, 0));
			/*nave->PintarModulo(0,0,0,64,64);
			nave->PintarModulo(0,100,100);*/
			keys = SDL_GetKeyState(NULL);
			
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
				estado = Estado::ESTADO_MENU;
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

			fondo->Pintar();
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
					if (enemigosEliminados<nivel[nivelActual].NumeroEnemigosEliminar)
					{
						enemigoArreglo[i]->crearNuevo();
					}
				}
			}
			/////////////////////////////////////////
			if (vida <= 0)
				estado = ESTADO_MENU;

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

			break;
		case Estado::ESTADO_FINALIZANDO:
			estado = Estado::ESTADO_ESPERANDO;
			salirJuego = false;
			break;
		case Estado::ESTADO_TERMINANDO:
			estado = Estado::ESTADO_MENU;
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
		SDL_Flip(screen);

		//Calculando fps
		tiempoFrameFinal = SDL_GetTicks();
		while (tiempoFrameFinal < (tiempoFrameInicial + FPS_DELAY))
		{
			tiempoFrameFinal = SDL_GetTicks();
			SDL_Delay(1);
		}
	   printf("Frames:%d Tiempo:%d Tiempo Promedio;%f Tiempo por Fame:%d FPS:%f\n", tick, SDL_GetTicks(), (float)SDL_GetTicks() / (float)tick, tiempoFrameFinal - tiempoFrameInicial, 1000.0f / (float)(tiempoFrameFinal - tiempoFrameInicial));
	   tiempoFrameInicial = tiempoFrameFinal;
	   tick++;

    }
	return true;
}

bool CGame::LimitePantalla(Objeto*objeto, int bandera)
{
	if(bandera & BORDE_IZQUIERDO)
	if(objeto->ObtenerX() <= 0)
		return true;
	if (bandera & BORDE_SUPERIOR)
	if (objeto->ObtenerY() <=0)
		return true;

	if (bandera & BORDE_DERECHO)
	if (objeto->ObtenerX() >=(WIDTH_SCREEN - objeto->ObtenerW()))
		return true;
	if (bandera & BORDE_INFERIOR)
	if (objeto->ObtenerY() >=HEIGHT_SCREEN - objeto->ObtenerH())
		return true;
	return false;

}//Termina LimitePantalla

void CGame::MoverEnemigo(){

	for (int i = 0; i < nivel[nivelActual].NumeroEnemigosVisibles; i++)
			{
				if(enemigoArreglo[i]->GetNaveObjeto()->ObtenerPasoActual()==0)
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


void CGame::Menu() 
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
			textos->Pintar(i + 2, 320, 220 + (j * 30)); 
		else 
			textos->Pintar(i, 320, 220 + (j * 30));

		if (keys[SDLK_RETURN]) 
		{
			if (opcionSeleccionada == MODULO_TEXTO_MENU_OPCION1) 
			{ 
				estado = Estado::ESTADO_PRE_JUGANDO; 
		    }

			if (opcionSeleccionada == MODULO_TEXTO_MENU_OPCION2) 
			{
				estado = Estado::ESTADO_FINALIZANDO; 
			}
		}// sdlk_return 
	}//for 
}//void	