#ifndef _SPRITE_H_
#define _SPRITE_H_
#include <SDL.h>
#include "SpriteDeF.h"
#include "OpegGLImplement.h"
#include "SDL_opengl.h"

class Sprite{
	SDL_Surface * imagen;//La imagen a manipular
	SpriteDef spriteDef;//definiciones de sprite
public:
	Sprite(OpenGlImplement *openGlImplement);//constructor recibe la pantalla
	~Sprite();
	void cargarimagen (char * nombre);
	/*void PintarModulo(int nombre, int x, int y, int w, int h);*/
	void DrawModulo(int nombre, int x, int y);
	int WidthModule(int module);
	int HeightModule(int module);
	/////////// OPENGL
	void Draw(SDL_Window *window, GLuint texture, GLfloat * texcoord);
	SDL_Window *window;//la pantalla
	GLuint SDL_GL_LoadTexture(SDL_Surface * surface, GLfloat * texcoord);
	int power_of_two(int input);

	GLuint texture;
	GLfloat texcoords[4];
	//////////////////
};


#endif