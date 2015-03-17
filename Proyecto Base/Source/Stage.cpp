#include "Game.h"

void CGame::InicializandoStage(){
	nivel[0].fondoNivel = 0;
	nivel[0].balasMaximas = 5;
	nivel[0].NumeroEnemigosEliminar = 5;
	nivel[0].NumeroEnemigosVisibles=2;
	nivel[0].velocidadBalasEnemigo = 3;
	nivel[0].velocidadBalasPropoias = 5;
	nivel[0].VelocidadNaveEnemigo = 4;
	nivel[0].VelocidadNavePropia = 4;
	nivel[0].vidaEnemigo = 1;

	nivel[1].fondoNivel = 1;
	nivel[1].balasMaximas = 5;
	nivel[1].NumeroEnemigosEliminar = 20;
	nivel[1].NumeroEnemigosVisibles = 4;
	nivel[1].velocidadBalasEnemigo = 6;
	nivel[1].velocidadBalasPropoias = 9;
	nivel[1].VelocidadNaveEnemigo = 13;
	nivel[1].VelocidadNavePropia = 6;
	nivel[1].vidaEnemigo = 1;
}