#pragma once
//#include <SDL_image.h>
#include "Objeto.h"
#include "Nave.h"
#include "Stage.h"



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
	void CargandoObjetos();
	void IniciandoVideo();
	void MoverEnemigo();
	void MenuPintar();
	void MenuActualizar();
	void JugandoPintar();
	void JugandoActualizar();
	bool LimitePantalla(Objeto * objeto, int bandera);
	void InicializandoStage();


	Uint8 *keys;//Esta variable nos servira para ver si determinadas teclas estan o no pulsadas
	SDL_Event event;//La variable event de tipo evento de SDL nos servira para motorizar
	SDL_Surface *screenBuffer;

	int opcionSeleccionada;
	int nivelActual;
	int vida;
	int enemigosEliminados;

	Nave *nave;
	Nave *enemigoArreglo[5];
	Stage nivel[4];
	
	Estado estadoJuego;
	
	int tick;
	int tiempoFrameInicial;
	int tiempoFrameFinal;

	Objeto *menuObjeto;//Fondo del menu
	Objeto *textosObjeto;//Textos del juego
	Objeto *fondoObjeto;//Fondo del juego

	

};