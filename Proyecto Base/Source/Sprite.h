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
	void loadTexture (char * name);
	
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
	void extractOBJdata(std::string fp, GLuint indexs);

	OpenGlImplement *openGlImplement;

	GLuint vertexBufferObject = 0;
	GLuint indexBufferObject = 0;
	GLuint textureBufferObject = 0;
	GLuint textureObject = 0;

	GLfloat* vexterPositions; 
	GLfloat* vertexTextures; 
	GLuint* vextexIndex;
	GLfloat* vextexNormals;  
	GLuint faces[][9];              // PTN PTN PTN

	//////////////////
};


#endif