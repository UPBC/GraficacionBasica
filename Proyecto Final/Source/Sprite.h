#ifndef _SPRITE_H_
#define _SPRITE_H_
#include <SDL.h>
#include "SpriteDeF.h"
#include "OpegGLImplement.h"
#include "Config.h"
#include "SDL_opengl.h"

class Sprite{
	int w;
	int h;

	int pasos;
	int pasoActual;
	int pasoLimite;
	bool isVisible;
	bool automovimiento;
	
	SpriteDef spriteDef;//definiciones de sprite

public:
	enum Coordenada{
		COORD_ROTAR_X,
		COORD_ROTAR_Y,
		COORD_ROTAR_Z,
		COORD_NO_ROTAR_X,
		COORD_NO_ROTAR_Y,
		COORD_NO_ROTAR_Z
	};

	Sprite(OpenGlImplement *openGlImplement);//constructor recibe la pantalla
	~Sprite();
	void CreateTextures (char * name);
	
	int WidthModule(int module);
	int HeightModule(int module);

	void SetVisible(bool isVisible);
	void MoverArribaAbajo(int posicion);

	Sprite(OpenGlImplement* openGlImplement, char * nameResource, int x, int y);

	void SetAutoMovimiento(bool automovimiento);
	void SetPasoLimite(int pasos);
	int ObtenerPasoActual();
	void IncrementarPasoActual();
	//bool EstaColicionando(Objeto * b);
	void Draw();
	
	void Translate(GLfloat x, GLfloat y, GLfloat z);
	void Translate(GLfloat x, GLfloat y);
	void Translate(GLfloat z);
	void TranslateDraw(GLfloat x, GLfloat y);
	void Rotate(GLfloat x, GLfloat y, GLfloat z);
	void Rotate(GLint);
	void Rotate();
	void Scale(GLfloat x, GLfloat y, GLfloat z);

	GLfloat rotate_x;
	GLfloat rotate_y;
	GLfloat rotate_z;
	GLfloat auto_rotate_x;
	GLfloat auto_rotate_y;
	GLfloat auto_rotate_z;
	GLboolean auto_enable_rotate_x;
	GLboolean auto_enable_rotate_y;
	GLboolean auto_enable_rotate_z;
	GLfloat translate_x;
	GLfloat translate_y;
	GLfloat translate_z;
	GLfloat scale_x;
	GLfloat scale_y;
	GLfloat scale_z;

	GLuint numero_vertices;

	void MoverLados(int posicion);
	void Actualizar();
	int GetY();
	int GetX();
	int GetW();
	int GetH();

	/////////// OPENGL
	// Model Structure
	typedef struct Model
	{
		int vertices;
		int positions;
		int texels;
		int normals;
		int faces;
		int modules;
	}
	Model;

	Model GetOBJinfo(std::string fp);
	void ExtractOBJdata(std::string fp, GLuint indexs);

	OpenGlImplement *openGlImplement;

	GLuint* vertexBufferObject;
	GLuint* indexBufferObject;
	GLuint* textureBufferObject;
	GLuint* textureObject;

	GLfloat* vertexPositions; 
	GLfloat* vertexTextures; 
	GLuint* vertexIndex;

	GLfloat* vertexNormals;  
	GLuint faces[][9];              // PTN PTN PTN

	//////////////////
};


#endif