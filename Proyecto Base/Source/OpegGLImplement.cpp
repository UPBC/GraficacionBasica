#include "OpegGLImplement.h"

OpenGlImplement::OpenGlImplement(){
}

void OpenGlImplement::setSDLWindow(SDL_Window *window){
	this->window = window;
}

void OpenGlImplement::InitGL()
{
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)SDL_GL_GetProcAddress("glAttachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)SDL_GL_GetProcAddress("glUseProgram");
	glCreateShader = (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
	glBindBuffer = (PFNGLBINDBUFFERPROC)SDL_GL_GetProcAddress("glBindBuffer");
	glGenBuffers = (PFNGLGENBUFFERSPROC)SDL_GL_GetProcAddress("glGenBuffers");
	glBufferData = (PFNGLBUFFERDATAPROC)SDL_GL_GetProcAddress("glBufferData");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)SDL_GL_GetProcAddress("glDeleteProgram");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glDisableVertexAttribArray");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)SDL_GL_GetProcAddress("glGetAttribLocation");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)SDL_GL_GetProcAddress("glGetProgramiv");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)SDL_GL_GetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress("glGetShaderInfoLog");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)SDL_GL_GetProcAddress("glGetProgramInfoLog");
	glIsShader = (PFNGLISSHADERPROC)SDL_GL_GetProcAddress("glIsShader");
	glIsProgram = (PFNGLISPROGRAMPROC)SDL_GL_GetProcAddress("glIsProgram");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)SDL_GL_GetProcAddress("glVertexAttribPointer");
}

void OpenGlImplement::InitShaders()
{
	//Success flag
	bool success = true;

	//Generate program
	shaderProgram = glCreateProgram();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read shaders
	std::string vertShaderStr = readFile("vertex.glsl");
	std::string fragShaderStr = readFile("fragment.glsl");
	const GLchar *vertexShaderSource = vertShaderStr.c_str();
	const GLchar *fragmentShaderSource = fragShaderStr.c_str();
		
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
		printf("Unable to compile vertex shader %d!\n", vertexShader);
	glAttachShader(shaderProgram, vertexShader);


	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
	if (fShaderCompiled != GL_TRUE)
		printf("Unable to compile fragment shader %d!\n", fragmentShader);
	glAttachShader(shaderProgram, fragmentShader);


	glLinkProgram(shaderProgram);
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programSuccess);
		
	if (programSuccess != GL_TRUE)
		printf("Error linking program %d!\n", shaderProgram);
		
	//Get vertex attribute location
	vertexPositionAttribute = glGetAttribLocation(shaderProgram, "aVertexPosition");
	
	//Initialize clear color
	glClearColor(0.f, 0.f, 0.f, 1.f);
	
}

void OpenGlImplement::InitBuffers(){
	//VBO data
	GLfloat vertexData[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	//IBO data
	GLuint indexData[] = { 0, 1, 2, 3 };
	//Create VBO
	glGenBuffers(1, &gVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	//Create IBO
	glGenBuffers(1, &gIndexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

}

void OpenGlImplement::QuitShaders()
{
	//Deallocate program
	glDeleteProgram(shaderProgram);
	//Destroy window	
	SDL_DestroyWindow(window);
	window = NULL;
	//Quit SDL subsystems
	SDL_Quit();
}

void OpenGlImplement::DrawStart()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();                /* Reset The View */
}

void OpenGlImplement::DrawEnd()
{
	SDL_GL_SwapWindow(window);
}

/* The main drawing function. */
void OpenGlImplement::Draw(GLuint texture, GLfloat * texcoord)
{
	glTranslatef(-1.5f, 0.0f, 0.0f);        /* Move Left 1.5 Units */


	//Bind program
	glUseProgram(shaderProgram);

	//Enable vertex position
	glEnableVertexAttribArray(vertexPositionAttribute);

	//Set vertex data
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
	glVertexAttribPointer(vertexPositionAttribute, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

	//Set index data and render
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferObject);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

	//Disable vertex position
	glDisableVertexAttribArray(vertexPositionAttribute);

	//Unbind program
	glUseProgram(NULL);

}

OpenGlImplement::~OpenGlImplement(){
	glDeleteProgram(shaderProgram);
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}

std::string OpenGlImplement::readFile(const char *filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}