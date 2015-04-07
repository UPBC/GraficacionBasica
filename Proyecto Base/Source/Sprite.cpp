#include "Sprite.h"
#include <stdio.h>
#include <SDL.h>

Sprite::Sprite(OpenGlImplement *openGlImplement){
	angle_x = 0;
	this->openGlImplement = openGlImplement;
}

Sprite::~Sprite()
{
	SDL_FreeSurface(imagen);
}

void Sprite::CreateTextures(char* name){
	imagen = SDL_LoadBMP(name);

	if (!imagen)
	{
		printf("No se ha podido cargar la imagen: %s\n", SDL_GetError());
		SDL_Quit();
		exit(3);
	}

	textureObject[0] = openGlImplement->LoadTexture(imagen, vextexPositions[0], vextexPositions[1], vextexPositions[0], vextexPositions[1]);

	SDL_FreeSurface(imagen);
}

void Sprite::DrawModulo(GLuint idArray, GLfloat x, GLfloat y, GLfloat z){
	openGlImplement->Draw(vertexBufferObject, indexBufferObject, textureBufferObject, textureObject[idArray], x, y, z, angle_x, numero_vertices);
}

	int Sprite::WidthModule(int module){
		return spriteDef.modulos[module].w;
	}

	int Sprite::HeightModule(int module){
		return spriteDef.modulos[module].h;
	}

	Sprite::Sprite(OpenGlImplement* openGlImplement, char * nameResource, int x, int y)
	{
		char pathImg[40];  
		char pathDat[40]; 

		strcpy(pathImg, nameResource);
		strcpy(pathDat, nameResource);
		strcat(pathImg, ".bmp");
		strcat(pathDat, ".dat");

		this->openGlImplement = openGlImplement;

		vertexBufferObject = new GLuint[1];
		indexBufferObject = new GLuint[1];
		textureBufferObject = new GLuint[1];
		textureObject = new GLuint[1];

		Model model = GetOBJinfo(pathDat);
		numero_vertices = model.positions;
		vextexPositions = new GLfloat[model.positions * 3];
		vertexTextures = new GLfloat[model.texels * 2];
		vextexIndex = new GLuint[model.positions];
		vextexNormals = new GLfloat[model.texels * 3];
		
		ExtractOBJdata(pathDat, model.positions);
		CreateTextures(pathImg);

		w = WidthModule(0);
		h = HeightModule(0);
		this->x = x;
		this->y = y;
		automovimiento = false;
		pasoActual = 0;
		pasoLimite = -1;
		angle_x = 0;

		faces[model.faces][9];

		openGlImplement->InitBuffers(vertexBufferObject, indexBufferObject, textureBufferObject, vextexPositions, 3 * model.positions * sizeof(vextexPositions), vextexIndex, model.positions * sizeof(GLuint), vertexTextures, 2 * model.texels * sizeof(vertexTextures));
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
		DrawModulo(0, x, y, 0.f);
	}

	void Sprite::Draw(GLuint idArray, GLfloat x, GLfloat y, GLfloat z){
		DrawModulo(idArray, x, y, z);
	}

	void Sprite::Rotate(){
		angle_x++;
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

	Sprite::Model Sprite::GetOBJinfo(std::string fp)
	{
		Model model = { 0 };

		// Open OBJ file
		std::ifstream inOBJ;
		inOBJ.open(fp);
		if (!inOBJ.good())
		{
			exit(1);
		}

		// Read OBJ file
		while (!inOBJ.eof())
		{
			std::string line;
			getline(inOBJ, line);
			std::string type = line.substr(0, 2);

			if (type.compare("v ") == 0)
				model.positions++;
			else if (type.compare("vt") == 0)
				model.texels++;
			else if (type.compare("vn") == 0)
				model.normals++;
			else if (type.compare("f ") == 0)
				model.faces++;
			else if (type.compare("m ") == 0)
				model.modules++;
		}

		model.vertices = model.faces * 3;

		// Close OBJ file
		inOBJ.close();

		return model;
	}

	void Sprite::ExtractOBJdata(std::string fp, GLuint indexes)
	{
		// Counters
		int p = 0;
		int t = 0;
		int n = 0;
		int f = 0;
		int in = 0;
		int m = 0;

		// Open OBJ file
		std::ifstream inOBJ;
		inOBJ.open(fp);
		if (!inOBJ.good())
		{
			exit(1);
		}

		// Read OBJ file
		while (!inOBJ.eof())
		{
			std::string line;
			getline(inOBJ, line);
			std::string type = line.substr(0, 2);

			// Positions
			if (type.compare("v ") == 0)
			{
				// Copy line for parsing
				char* l = new char[line.size() + 1];
				memcpy(l, line.c_str(), line.size() + 1);

				// Extract tokens
				strtok(l, " ");
				for (int i = 0; i < 3; i++)
					vextexPositions[(p * 3) + i] = openGlImplement->ConvertCOORDf(atoi(strtok(NULL, " ")));

				// Wrap up
				delete[] l;
				p++;
			}

			// Texels
			else if (type.compare("vt") == 0)
			{
				// Copy line for parsing
				char* l = new char[line.size() + 1];
				memcpy(l, line.c_str(), line.size() + 1);

				// Extract tokens
				strtok(l, " ");
				for (int i = 0; i < 2; i++)
					vertexTextures[(t * 2) + i] = atof(strtok(NULL, " "));
					
				// Wrap up
				delete[] l;
				t++;
			}
			// Index
			else if (type.compare("i ") == 0)
			{
				// Copy line for parsing
				char* l = new char[line.size() + 1];
				memcpy(l, line.c_str(), line.size() + 1);

				// Extract tokens
				strtok(l, " ");
				for (int i = 0; i < indexes; i++)
					vextexIndex[(in * 1) + i] = atoi(strtok(NULL, " "));
				
				// Wrap up
				delete[] l;
				in++;
			}
			// Modules
			else if (type.compare("m ") == 0)
			{
				
			}
			// Normals
			else if (type.compare("vn") == 0)
			{
			}

			// Faces
			else if (type.compare("f ") == 0)
			{
			}
		}

		// Close OBJ file
		inOBJ.close();
	}