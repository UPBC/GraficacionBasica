#pragma once
#include <SDL_image.h>
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
	void Iniciando();
	void MoverEnemigo();
	void Menu();
	bool LimitePantalla(Objeto * objeto, int bandera);
	unsigned int x;

	int opcionSeleccionada;
	Uint8 *keys;//Esta variable nos servira para ver si determinadas teclas estan o no pulsadas
	SDL_Event event;//La variable event de tipo evento de SDL nos servira para motorizar
	SDL_Surface *screen, *image;

	Nave *nave;
	Nave *enemigoArreglo[5];
	Stage nivel[4];
	void Inicializandotage();
	int nivelActual;

	Estado estado;
	Estado condicion;

	int tick;
	int tiempoFrameInicial;
	int tiempoFrameFinal;

	Objeto *menu;//Fondo del menu
	Objeto *textos;//Textos del juego
	Objeto *fondo;//Fondo del juego

	int vida;
	int enemigosEliminados;

};