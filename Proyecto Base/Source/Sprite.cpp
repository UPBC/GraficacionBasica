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

void Sprite::loadTexture(char* name){
	imagen = SDL_LoadBMP(name);
	if (!imagen)
	{
		printf("No se ha podido cargar la imagen: %s\n", SDL_GetError());
		SDL_Quit();
		exit(3);
	}

	textureObject = openGlImplement->LoadTexture(imagen);
	SDL_FreeSurface(imagen);

}

void Sprite::DrawModulo(int nombre, int x, int y){
	SDL_Rect src;
	src.x = spriteDef.modulos[nombre].x;
	src.y = spriteDef.modulos[nombre].y;
	src.w = spriteDef.modulos[nombre].w;
	src.h = spriteDef.modulos[nombre].h;
	SDL_Rect dest;
	dest.y = y;
	dest.x = x;
	
	openGlImplement->Draw(&vertexBufferObject, &indexBufferObject, &textureBufferObject, textureObject, x, y);
}

	int Sprite::WidthModule(int module){
		return spriteDef.modulos[module].w;
	}

	int Sprite::HeightModule(int module){
		return spriteDef.modulos[module].h;
	}

	Sprite::Sprite(OpenGlImplement* openGlImplement, char * nameResource, int x, int y, int module)
	{
		char pathImg[40];  
		char pathDat[40]; 

		strcpy(pathImg, nameResource);
		strcpy(pathDat, nameResource);
		strcat(pathImg, ".bmp");
		strcat(pathDat, ".dat");

		this->module = module;
		this->openGlImplement = openGlImplement;
		loadTexture(pathImg);
		w = WidthModule(this->module);
		h = HeightModule(this->module);
		this->x = x;
		this->y = y;
		automovimiento = false;
		pasoActual = 0;
		pasoLimite = -1;

		Model model = getOBJinfo(pathDat);
		
		vexterPositions = new GLfloat[model.positions * 3];
		vertexTextures = new GLfloat[model.texels * 2];
		vextexIndex = new GLuint[model.positions];
		vextexNormals = new GLfloat[model.texels * 3];

		faces[model.faces][9];

		extractOBJdata(pathDat, model.positions);
		openGlImplement->InitBuffers(&vertexBufferObject, &indexBufferObject, &textureBufferObject, vexterPositions, 3 * model.positions * sizeof(vexterPositions), vextexIndex, model.positions * sizeof(GLuint), vertexTextures, 2 * model.texels * sizeof(vertexTextures));
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

	Sprite::Model Sprite::getOBJinfo(std::string fp)
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
		}

		model.vertices = model.faces * 3;

		// Close OBJ file
		inOBJ.close();

		return model;
	}

	void Sprite::extractOBJdata(std::string fp, GLuint indexes)
	{
		// Counters
		int p = 0;
		int t = 0;
		int n = 0;
		int f = 0;
		int in = 0;

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
				for (int i = 0; i < 3; i++){
					vexterPositions[(p*3)+i] = atof(strtok(NULL, " "));
					//printf("%f ", vexterPositions[(p * 3) + i]);
				}
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
				for (int i = 0; i < 2; i++){
					vertexTextures[(t * 2) + i] = atof(strtok(NULL, " "));
					//printf("%f ", vertexTextures[(t * 2) + i]);
				}
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
				for (int i = 0; i < indexes; i++){
					vextexIndex[(in * 1) + i] = atoi(strtok(NULL, " "));
				}
				// Wrap up
				delete[] l;
				in++;
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