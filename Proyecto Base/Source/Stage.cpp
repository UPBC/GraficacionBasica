#include "Game.h"

void CGame::InicializandoStage(){
	nivel[0].fondoNivel = 0;
	nivel[0].balasMaximas = 3;
	nivel[0].NumeroEnemigosEliminar = 10;
	nivel[0].NumeroEnemigosVisibles=2;
	nivel[0].velocidadBalasEnemigo = 5;
	nivel[0].velocidadBalasPropoias = 10;
	nivel[0].VelocidadNaveEnemigo = 20;
	nivel[0].VelocidadNavePropia = 5;
	nivel[0].vidaEnemigo = 1;

	nivel[0].fondoNivel = 1;
	nivel[0].balasMaximas = 2;
	nivel[0].NumeroEnemigosEliminar = 20;
	nivel[0].NumeroEnemigosVisibles = 4;
	nivel[0].velocidadBalasEnemigo = 6;
	nivel[0].velocidadBalasPropoias = 9;
	nivel[0].VelocidadNaveEnemigo = 13;
	nivel[0].VelocidadNavePropia = 6;
	nivel[0].vidaEnemigo = 1;
}