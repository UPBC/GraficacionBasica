#ifndef _SPRITE_H_
#define _SPRITE_H_
#include <SDL.h>
#include "SpriteDeF.h"
#include "OpegGLImplement.h"
#include "Config.h"
#include "SDL_opengl.h"

class Sprite{
	int x;
	int y;
	int w;
	int h;

	int pasos;
	int pasoActual;
	int pasoLimite;
	int module;//Modulo actual
	bool isVisible;
	bool automovimiento;
	
	SDL_Surface * imagen;//La imagen a manipular
	SpriteDef spriteDef;//definiciones de sprite

public:
	Sprite(OpenGlImplement *openGlImplement);//constructor recibe la pantalla
	~Sprite();
	void cargarimagen (char * nombre);
	
	void DrawModulo(int nombre, int x, int y);
	int WidthModule(int module);
	int HeightModule(int module);

	void SetVisible(bool isVisible);
	void MoverArribaAbajo(int posicion);

	Sprite(OpenGlImplement* openGlImplement, char * nameResource, int x, int y, int module);//

	void SetAutoMovimiento(bool automovimiento);
	void SetPasoLimite(int pasos);
	int ObtenerPasoActual();
	void IncrementarPasoActual();
	//bool EstaColicionando(Objeto * b);
	void Draw();
	void Draw(int modulo, int x, int y);
	void MoverLados(int posicion);
	void Actualizar();
	int GetY();
	int GetX();
	int GetW();
	int GetH();
	void SetXY(int x, int y);
	void SetX(int x);
	void SetY(int y);

	/////////// OPENGL
	// Model Structure
	typedef struct Model
	{
		int vertices;
		int positions;
		int texels;
		int normals;
		int faces;
	}
	Model;

	Model getOBJinfo(std::string fp);
	void extractOBJdata(std::string fp, GLfloat* vexterPositions, GLfloat** texels, GLfloat** normals, GLuint faces[][9]);

	OpenGlImplement *openGlImplement;
	GLuint SDL_GL_LoadTexture(SDL_Surface * surface, GLfloat * texcoord);
	int power_of_two(int input);

	GLuint texture;
	GLfloat texcoords[4];

	GLuint gVertexBufferObject = 0;
	GLuint gIndexBufferObject = 0;

	GLfloat* vexterPositions;    // XYZ
	GLfloat** texels;          // UV
	GLfloat** normals;        // XYZ
	GLuint faces[][9];              // PTN PTN PTN

	//////////////////
};


#endif