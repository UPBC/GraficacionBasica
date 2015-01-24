#include "Objeto.h"
#include "Config.h"

Objeto::Objeto(SDL_Surface * screen, char * rutaImagen, int x, int y, int module)
{
	this->module = module;
	sprite = new Sprite(screen);
	sprite->cargarimagen(rutaImagen);
	w = sprite->WidthModule(this->module);
	h = sprite->HeightModule(this->module);
	this->x = x;
	this->y = y;
	automovimiento = false;
	pasoActual=0;
	pasoLimite=-1;
}

void Objeto::SetAutoMovimiento(bool automovimiento)
{
	this->automovimiento = automovimiento;
}

void Objeto::Actualizar()
{
	if (automovimiento)
	{
		MoverLados(1);
		MoverArribaAbajo(1);
	}
	if (pasoLimite>0)
	{
		//pasoActual++;
		if (pasoActual>=pasoLimite)
		pasoActual=0;
	}	
	}

void Objeto::Pintar(){
	sprite->PintarModulo(this->module,x,y);
}

void Objeto::Pintar(int modulo, int x, int y){
	sprite->PintarModulo(modulo, x, y);
}

void Objeto::SetVisible(bool isVisible)
{
	this->isVisible = isVisible;
}

void Objeto::MoverLados(int posicion){
	x += posicion;
}

void Objeto::MoverArribaAbajo(int posicion)
{
	y += posicion;
}


int Objeto::ObtenerX(){
	return x;
}

int Objeto::ObtenerY(){
	return y;

}

int Objeto::ObtenerW()
{
	return w;
}

int Objeto::ObtenerH()
{
	return h;
}

void Objeto::SetPasoLimite(int pasos)
{
	this->pasoLimite = pasos;
}

int Objeto::ObtenerPasoActual(){
	return pasoActual;
}

void Objeto::IncrementarPasoActual()
{
	pasoActual++;
}

void Objeto::ponerEn(int x, int y){
	this->x = x;
	this->y = y;
}

//bool Objeto::EstaColicionandObjetove * b)
//{
//}