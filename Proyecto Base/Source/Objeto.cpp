#include "Objeto.h"
#include "Config.h"

Objeto::Objeto(OpenGlImplement* openGlImplement, char * rutaImagen, int x, int y, int module)
{
	this->module = module;
	sprite = new Sprite(openGlImplement);
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

void Objeto::Draw(){
	sprite->DrawModulo(this->module,x,y);
}

void Objeto::Draw(int modulo, int x, int y){
	sprite->DrawModulo(modulo, x, y);
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


int Objeto::GetX(){
	return x;
}

int Objeto::GetY(){
	return y;

}

int Objeto::GetW()
{
	return w;
}

int Objeto::GetH()
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

void Objeto::SetXY(int x, int y){
	SetX(x);
	SetY(y);
}

void Objeto::SetX(int x){
	this->x = x;
}

void Objeto::SetY(int y){
	this->y = y;
}
//bool Objeto::EstaColicionandObjetove * b)
//{
//}