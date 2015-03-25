#include "Sprite.h"
#include <stdio.h>
#include <SDL.h>

Sprite::Sprite(OpenGlImplement *openGlImplement){
	this->openGlImplement = openGlImplement;
}

Sprite::~Sprite()
{
	SDL_FreeSurface(imagen);
}

void Sprite::cargarimagen(char*nombre){
	imagen = SDL_LoadBMP(nombre);	
	if (!imagen)
	{
		printf("No se ha podido cargar la imagen: %s\n", SDL_GetError());
		SDL_Quit();
		exit(3);
	}

	texture = SDL_GL_LoadTexture(imagen, texcoords);
	SDL_FreeSurface(imagen);

}
//void Sprite::PintarModulo(int color, int x, int y, int w, int h){
//	SDL_Rect src;
//	src.x = x;
//	src.y = y;
//	src.w = w;
//	src.h = h;
//
//	SDL_BlitSurface(imagen, &src, screen, NULL);
//}

	void Sprite::DrawModulo(int nombre, int x, int y){
	SDL_Rect src;
	src.x = spriteDef.modulos[nombre].x;
	src.y = spriteDef.modulos[nombre].y;
	src.w = spriteDef.modulos[nombre].w;
	src.h = spriteDef.modulos[nombre].h;
	SDL_Rect dest;
	dest.y = y;
	dest.x = x;
	//SDL_BlitSurface(imagen, &src, screen, &dest);// Estudiar
	openGlImplement->Draw(&gVertexBufferObject, &gIndexBufferObject);

}

	int Sprite::WidthModule(int module){
		return spriteDef.modulos[module].w;
	}

	int Sprite::HeightModule(int module){
		return spriteDef.modulos[module].h;
	}

	GLuint Sprite::SDL_GL_LoadTexture(SDL_Surface * surface, GLfloat * texcoord)
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


	/* Quick utility function for texture creation */
	int Sprite::power_of_two(int input)
	{
		int value = 1;

		while (value < input) {
			value <<= 1;
		}
		return value;
	}


	Sprite::Sprite(OpenGlImplement* openGlImplement, char * rutaImagen, int x, int y, int module)
	{
		this->module = module;
		this->openGlImplement = openGlImplement;
		cargarimagen(rutaImagen);
		w = WidthModule(this->module);
		h = HeightModule(this->module);
		this->x = x;
		this->y = y;
		automovimiento = false;
		pasoActual = 0;
		pasoLimite = -1;

		//VBO data
		GLfloat vertexData[] =
		{
			-0.3f, -0.3f, 0.0f,
			0.3f, -0.3f, 0.0f,
			0.3f, 0.3f, 0.0f,
			-0.3f, 0.3f, 0.0f
		};

		//IBO data
		GLuint indexData[] = { 0, 1, 2, 3 };
		openGlImplement->InitBuffers(&gVertexBufferObject, &gIndexBufferObject, vertexData, sizeof(vertexData), indexData, sizeof(indexData));
	}

	void Sprite::SetAutoMovimiento(bool automovimiento)
	{
		this->automovimiento = automovimiento;
	}

	void Sprite::Actualizar()
	{
		if (automovimiento)
		{
			MoverLados(1);
			MoverArribaAbajo(1);
		}
		if (pasoLimite>0)
		{
			//pasoActual++;
			if (pasoActual >= pasoLimite)
				pasoActual = 0;
		}
	}

	void Sprite::Draw(){
		DrawModulo(this->module, x, y);
	}

	void Sprite::Draw(int modulo, int x, int y){
		DrawModulo(modulo, x, y);
	}

	void Sprite::SetVisible(bool isVisible)
	{
		this->isVisible = isVisible;
	}

	void Sprite::MoverLados(int posicion){
		x += posicion;
	}

	void Sprite::MoverArribaAbajo(int posicion)
	{
		y += posicion;
	}


	int Sprite::GetX(){
		return x;
	}

	int Sprite::GetY(){
		return y;

	}

	int Sprite::GetW()
	{
		return w;
	}

	int Sprite::GetH()
	{
		return h;
	}

	void Sprite::SetPasoLimite(int pasos)
	{
		this->pasoLimite = pasos;
	}

	int Sprite::ObtenerPasoActual(){
		return pasoActual;
	}

	void Sprite::IncrementarPasoActual()
	{
		pasoActual++;
	}

	void Sprite::SetXY(int x, int y){
		SetX(x);
		SetY(y);
	}

	void Sprite::SetX(int x){
		this->x = x;
	}

	void Sprite::SetY(int y){
		this->y = y;
	}