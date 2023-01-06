/*
Semestre 2023-1
Animación:
1.- Simple o básica:Por banderas y condicionales
2.- Compleja: Por medio de funciones y algoritmos,Textura Animada.
4.- Técnicas de Animación: Por Keyframes
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>r
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación

static float delay_daynight = 0.0f;
static float delay_helper = 0.0f;
static float light_changing = 1.0f;

bool day_flag = true;
int indexSkybox = 0.0f;


float toffsetu = 0.0f;
float toffsetv = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

//variables animación por KeyFrames
#include <iostream>
#include <fstream>
using namespace std;
float reproduciranimacion, habilitaranimacion, guardoFrame,
reinicioFrame, ciclo, ciclo2, contador = 0;
string KeyframesValues = "valores.txt";
ofstream archivo;
//finaliza keyframes

//Texturas
Texture pisoTexture;

//Modelos
Model Scenario_M;

//Partes Orca//
Model AletaDerOrca;
Model AletaIzqOrca;
Model ColaOrca;
Model CuerpoDelOrca;
Model CuerpoTraOrca;

//Partes Tortuga//
Model AletaDerTortuga;
Model AletaIzqTortuga;
Model AletaTraIzqTortuga;
Model AletaTraDerTortuga;
Model CabezaTortuga;
Model TorsoTortuga;

//Partes Tiburon//
Model AletaDerTiburon;
Model AletaIzqTiburon;
Model BocaTiburon;
Model ColaTiburon;
Model CuerpoDelTiburon;
Model CuerpoPrinTiburon;

//Partes Pinguino//
Model AletaDerPinguino;
Model AletaIzqPinguino;
Model CabezaPinguino;
Model PieDerPinguino;
Model PieIzqPinguino;
Model TorsoPinguino;

//Partes Manta//
Model AletaInfDerManta;
Model AletaInfIzqManta;
Model AletaSupDerManta;
Model AletaSupIzqManta;
Model CuerpoManta;
Model ColaManta;

//Partes Pez Cafe//
Model AletaDelDerPezCafe;
Model AletaDelIzqPezCafe;
Model AletaTraDerPezCafe;
Model AletaTraIzqPezCafe;
Model ColaPezCafe;
Model CuerpoPezCafe;

//Partes Pez Azul//
Model AletaDerPezAzul;
Model AletaIzqPezAzul;
Model ColaPezAzul;
Model CuerpoPezAzul;

//Partes Delfin//
Model AletaDerDelfin;
Model AletaIzqDelfin;
Model ColaDelfin;
Model CuerpoDelDelfin;
Model CuerpoTraDelfin;

Model acuario2;

Model estPingu;
Model AcuarioTortuga;
Model AcuarioPez;
Model AcuarioExt;

Model marco1;
Model marco2;
Model marco3;

Model reja;
Model camino;

//Skybox
Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	unsigned int pantallaIndices[] = {
		0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 7, 6, 5,
		5, 4, 7, 4, 0, 3, 3, 7, 4, 4, 5, 1, 1, 0, 4
	};

	unsigned int imagenIndices[] = { //Plano top
		3, 2, 6, 6, 7, 3
	};

	GLfloat animacion1Vertices[] = {
		-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.005f, 0.0f,	0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		0.005f, 1.0f,	0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	GLfloat pantallaVertices[] = {
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-15.0f, 0.0f, -15.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		15.0f, 0.0f, -15.0f,	15.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-15.0f, 0.0f, 15.0f,	0.0f, 15.0f,	0.0f, -1.0f, 0.0f,
		15.0f, 0.0f, 15.0f,		15.0f, 15.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	//textura piso
	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

// -------------------------------------------------------------------------------------------------------------//
//													KEYFRAMES										   		    //
// -------------------------------------------------------------------------------------------------------------// 
bool animacion = false;
float posAleFrontIzq = 0.0, posAleFrontDer = 0.0, posAleTrasIzq = 0.0, posAleTrasDer = 0.0, posBody = 0.0, rotBody = 0.0;
float movAleFrontIzq = 0.0, movAleFrontDer = 0.0, movAleTrasIzq = 0.0, movAleTrasDer = 0.0, movBody = 0.0, movRotBody = 0.0;

#define MAX_FRAMES 30
int i_max_steps = 90;
int i_curr_steps = 1;
typedef struct _frame
{
	float movAleFrontIzqT;
	float movAleFrontDerT;
	float movAleTrasIzqT;
	float movAleTrasDerT;
	float movBodyT;
	float rotBodyT;

	float movAleFrontIzqT_Inc;
	float movAleFrontDerT_Inc;
	float movAleTrasIzqT_Inc;
	float movAleTrasDerT_Inc;
	float movBodyT_Inc;
	float rotBodyT_Inc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 1;
bool play = false;
int playIndex = 0;

void saveFrame()
{
	printf("\tSaveFrame[%d: -> movAletaFrontalIzquierda:%f, movAletaFrontalDerecha:%f, movAletaTraseraIzquierda:%f, movAletaTraseraDerecha:%f, movCuerpo:%f, rotCuerpo:%f]\n",
		FrameIndex, movAleFrontIzq, movAleFrontDer, movAleTrasIzq, movAleTrasDer, movBody, movRotBody);

	KeyFrame[FrameIndex].movAleFrontDerT = movAleFrontDer;
	KeyFrame[FrameIndex].movAleFrontIzqT = movAleFrontIzq;
	KeyFrame[FrameIndex].movAleTrasDerT = movAleTrasDer;
	KeyFrame[FrameIndex].movAleTrasIzqT = movAleTrasIzq;
	KeyFrame[FrameIndex].movBodyT = movBody;
	KeyFrame[FrameIndex].rotBodyT = movRotBody;
	FrameIndex++;
}

void resetElements(void)
{
	movAleFrontIzq = KeyFrame[0].movAleFrontIzqT;
	movAleFrontDer = KeyFrame[0].movAleFrontDerT;
	movAleTrasDer = KeyFrame[0].movAleTrasDerT;
	movAleTrasIzq = KeyFrame[0].movAleTrasIzqT;
	movBody = KeyFrame[0].movBodyT;
	movRotBody = KeyFrame[0].rotBodyT;
}

void interpolation(void)
{
	KeyFrame[playIndex].movAleFrontDerT_Inc = (KeyFrame[playIndex + 1].movAleFrontDerT - KeyFrame[playIndex].movAleFrontDerT) / i_max_steps;
	KeyFrame[playIndex].movAleFrontIzqT_Inc = (KeyFrame[playIndex + 1].movAleFrontIzqT - KeyFrame[playIndex].movAleFrontIzqT) / i_max_steps;
	KeyFrame[playIndex].movAleTrasDerT_Inc = (KeyFrame[playIndex + 1].movAleTrasDerT - KeyFrame[playIndex].movAleTrasDerT) / i_max_steps;
	KeyFrame[playIndex].movAleTrasIzqT_Inc = (KeyFrame[playIndex + 1].movAleTrasIzqT - KeyFrame[playIndex].movAleTrasIzqT) / i_max_steps;
	KeyFrame[playIndex].movBodyT_Inc = (KeyFrame[playIndex + 1].movBodyT - KeyFrame[playIndex].movBodyT) / i_max_steps;
	KeyFrame[playIndex].rotBodyT_Inc = (KeyFrame[playIndex + 1].rotBodyT - KeyFrame[playIndex].rotBodyT) / i_max_steps;
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				habilitaranimacion = 0;
				//printf("\n Presiona 0 para habilitar reproducir de nuevo la animación'\n");
			}
			else
			{
				play = false;
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}	
	if (keys[GLFW_KEY_1]) //Aleta Frontal Derecha
	{
		if (ciclo < 1)
		{
			movAleFrontDer += 1.0f;
			ciclo++;
			ciclo2 = 0;
			//printf("\n reinicia con R\n");
		}
	}
	if (keys[GLFW_KEY_2]) //Aleta Frontal Izquierda
	{
		if (ciclo < 1)
		{
			movAleFrontIzq += 1.0f;
			ciclo++;
			ciclo2 = 0;
			//printf("\n reinicia con R\n");
		}
	}
	if (keys[GLFW_KEY_3]) //Aleta Trasera Derecha
	{
		if (ciclo < 1)
		{
			movAleTrasDer += 1.0f;
			ciclo++;
			ciclo2 = 0;
			//printf("\n reinicia con R\n");
		}
	}
	if (keys[GLFW_KEY_4]) //Aleta Trasera Izquierda
	{
		if (ciclo < 1)
		{
			movAleTrasIzq += 1.0f;
			ciclo++;
			ciclo2 = 0;
			//printf("\n reinicia con R\n");
		}
	}
	if (keys[GLFW_KEY_5]) //Movimiento del cuerpo (adelante)
	{
		if (ciclo < 1)
		{
			movBody += 1.0f;
			ciclo++;
			ciclo2 = 0;
			//printf("\n reinicia con 2\n");
		}
	}
	if (keys[GLFW_KEY_6]) //Movimiento del cuerpo (atrás)
	{
		if (ciclo < 1)
		{
			movBody -= 1.0f;
			ciclo++;
			ciclo2 = 0;
			//printf("\n reinicia con R\n");
		}
	}
	if (keys[GLFW_KEY_7]) //Rotación del cuerpo (izq)
	{
		if (ciclo < 1)
		{
			rotBody -= 1.0f;
			ciclo++;
			ciclo2 = 0;
			//printf("\n reinicia con R\n");
		}
	}
	if (keys[GLFW_KEY_8]) //Rotación del cuerpo (der)
	{
		if (ciclo < 1)
		{
			rotBody += 1.0f;
			ciclo++;
			ciclo2 = 0;
			//printf("\n reinicia con R\n");
		}
	}
	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			//printf(" \nPresiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
		}
	}
	if (keys[GLFW_KEY_R])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
		}
	}
}

void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps)
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina animacion\n");
				playIndex = 0;
				play = false;
			}
			else
			{
				i_curr_steps = 0;
				interpolation();
			}
		}
		else
		{
			movAleFrontDer += KeyFrame[playIndex].movAleFrontDerT;
			movAleFrontIzq += KeyFrame[playIndex].movAleFrontIzqT;
			movAleTrasDer += KeyFrame[playIndex].movAleTrasDerT;
			movAleTrasIzq += KeyFrame[playIndex].movAleTrasIzqT;
			movBody += KeyFrame[playIndex].movBodyT;
			rotBody += KeyFrame[playIndex].movBodyT;
			i_curr_steps++;
		}
	}
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	camera = Camera(glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 2.5f, 0.5f);

	//Carga de texturas
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	std::vector<std::string> skyboxFaces;
	std::vector<std::string> nowSkybox;

	//light_1.0
	skyboxFaces.push_back("Textures/Skybox/town_light_1_0/town_px.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_1_0/town_nx.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_1_0/town_ny.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_1_0/town_py.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_1_0/town_pz.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_1_0/town_nz.png");
	//light_0.9
	skyboxFaces.push_back("Textures/Skybox/town_light_0_9/town_px.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_9/town_nx.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_9/town_ny.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_9/town_py.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_9/town_pz.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_9/town_nz.png");
	//light_0.8
	skyboxFaces.push_back("Textures/Skybox/town_light_0_8/town_px.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_8/town_nx.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_8/town_ny.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_8/town_py.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_8/town_pz.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_8/town_nz.png");
	//light_0.7
	skyboxFaces.push_back("Textures/Skybox/town_light_0_7/town_px.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_7/town_nx.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_7/town_ny.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_7/town_py.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_7/town_pz.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_7/town_nz.png");
	//light_0.6
	skyboxFaces.push_back("Textures/Skybox/town_light_0_6/town_px.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_6/town_nx.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_6/town_ny.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_6/town_py.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_6/town_pz.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_6/town_nz.png");
	//light_0.5
	skyboxFaces.push_back("Textures/Skybox/town_light_0_5/town_px.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_5/town_nx.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_5/town_ny.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_5/town_py.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_5/town_pz.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_5/town_nz.png");
	//light_0.4
	skyboxFaces.push_back("Textures/Skybox/town_light_0_4/town_px.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_4/town_nx.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_4/town_ny.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_4/town_py.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_4/town_pz.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_4/town_nz.png");
	//light_0.3
	skyboxFaces.push_back("Textures/Skybox/town_light_0_3/town_px.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_3/town_nx.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_3/town_ny.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_3/town_py.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_3/town_pz.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_3/town_nz.png");
	//light_0.2
	skyboxFaces.push_back("Textures/Skybox/town_light_0_2/town_px.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_2/town_nx.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_2/town_ny.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_2/town_py.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_2/town_pz.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_2/town_nz.png");
	//light_0.1
	skyboxFaces.push_back("Textures/Skybox/town_light_0_1/town_px.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_1/town_nx.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_1/town_ny.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_1/town_py.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_1/town_pz.png");
	skyboxFaces.push_back("Textures/Skybox/town_light_0_1/town_nz.png");

	//Carga de modelos// 

	// ACUARIO //
	acuario2 = Model();
	acuario2.LoadModel("Models/aquarium2/acuario.obj");

	// EstanquePingu //
	estPingu = Model();
	estPingu.LoadModel("Models/AcuarioPinguino/AcuaPingu.obj");

	// AcuarioTortuga //
	AcuarioTortuga = Model();
	AcuarioTortuga.LoadModel("Models/AcuarioTortuga/AcuarioTortuga.obj");

	// AcuarioPez //
	AcuarioPez = Model();
	AcuarioPez.LoadModel("Models/AcuarioPez/AcuarioPez.obj");

	// AcuarioExt //
	AcuarioExt = Model();
	AcuarioExt.LoadModel("Models/AcuarioExt/AcuarioExt.obj");

	// marco1 //
	marco1 = Model();
	marco1.LoadModel("Models/marcos/marco1.obj");

	// marco2 //
	marco2 = Model();
	marco2.LoadModel("Models/marcos/marco2.obj");

	// marco3 //
	marco3 = Model();
	marco3.LoadModel("Models/marcos/marco3.obj");

	// reja //
	reja = Model();
	reja.LoadModel("Models/reja/reja.obj");

	// reja //
	camino = Model();
	camino.LoadModel("Models/camino/camino.obj");

	//ORCA//
	AletaDerOrca = Model();
	AletaDerOrca.LoadModel("Models/orca/AletaDerOrca.obj");
	AletaIzqOrca = Model();
	AletaIzqOrca.LoadModel("Models/orca/AletaIzqOrca.obj");
	ColaOrca = Model();
	ColaOrca.LoadModel("Models/orca/ColaOrca.obj");
	CuerpoDelOrca = Model();
	CuerpoDelOrca.LoadModel("Models/orca/CuerpoDelOrca.obj");
	CuerpoTraOrca = Model();
	CuerpoTraOrca.LoadModel("Models/orca/CuerpoTraOrca.obj");

	//TORTUGA//
	AletaDerTortuga = Model();
	AletaDerTortuga.LoadModel("Models/turtle/AletaDer.obj");
	AletaIzqTortuga = Model();
	AletaIzqTortuga.LoadModel("Models/turtle/AletaIzq.obj");
	AletaTraDerTortuga = Model();
	AletaTraDerTortuga.LoadModel("Models/turtle/AletaTraDer.obj");
	AletaTraIzqTortuga = Model();
	AletaTraIzqTortuga.LoadModel("Models/turtle/AletaTraIzq.obj");
	CabezaTortuga = Model();
	CabezaTortuga.LoadModel("Models/turtle/cabezaTortuga.obj");
	TorsoTortuga = Model();
	TorsoTortuga.LoadModel("Models/turtle/torsoTortuga.obj");

	//TIBURON//
	AletaDerTiburon = Model();
	AletaDerTiburon.LoadModel("Models/shark/AletaDerTiburon.obj");
	AletaIzqTiburon = Model();
	AletaIzqTiburon.LoadModel("Models/shark/AletaIzqTiburon.obj");
	BocaTiburon = Model();
	BocaTiburon.LoadModel("Models/shark/BocaTiburon.obj");
	ColaTiburon = Model();
	ColaTiburon.LoadModel("Models/shark/ColaTiburon.obj");
	CuerpoDelTiburon = Model();
	CuerpoDelTiburon.LoadModel("Models/shark/CuerpoDelTiburon.obj");
	CuerpoPrinTiburon = Model();
	CuerpoPrinTiburon.LoadModel("Models/shark/CuerpoPrinTiburon.obj");

	//PINGUINO//
	AletaDerPinguino = Model();
	AletaDerPinguino.LoadModel("Models/penguin/AletaDerPinguino.obj");
	AletaIzqPinguino = Model();
	AletaIzqPinguino.LoadModel("Models/penguin/AletaIzqPinguino.obj");
	CabezaPinguino = Model();
	CabezaPinguino.LoadModel("Models/penguin/CabezaPinguino.obj");
	PieDerPinguino = Model();
	PieDerPinguino.LoadModel("Models/penguin/PieDerPinguino.obj");
	PieIzqPinguino = Model();
	PieIzqPinguino.LoadModel("Models/penguin/PieIzqPinguino.obj");
	TorsoPinguino = Model();
	TorsoPinguino.LoadModel("Models/penguin/TorsoPinguino.obj");

	//Manta//
	AletaInfDerManta = Model();
	AletaInfDerManta.LoadModel("Models/manta/AletaInfDerManta.obj");
	AletaInfIzqManta = Model();
	AletaInfIzqManta.LoadModel("Models/manta/AletaInfIzqManta.obj");
	AletaSupDerManta = Model();
	AletaSupDerManta.LoadModel("Models/manta/AletaSupDerManta.obj");
	AletaSupIzqManta = Model();
	AletaSupIzqManta.LoadModel("Models/manta/AletaSupIzqManta.obj");
	CuerpoManta = Model();
	CuerpoManta.LoadModel("Models/manta/CuerpoManta.obj");
	ColaManta = Model();
	ColaManta.LoadModel("Models/manta/ColaManta.obj");

	//Pez Cafe//
	 AletaDelDerPezCafe = Model();
	 AletaDelDerPezCafe.LoadModel("Models/fish/Cafe/AletaDelDerPezCafe.obj");
	 AletaDelIzqPezCafe = Model();
	 AletaDelIzqPezCafe.LoadModel("Models/fish/Cafe/AletaDelIzqPezCafe.obj");
	 AletaTraDerPezCafe = Model();
	 AletaTraDerPezCafe.LoadModel("Models/fish/Cafe/AletaTraDerPezCafe.obj");
	 AletaTraIzqPezCafe = Model();
	 AletaTraIzqPezCafe.LoadModel("Models/fish/Cafe/AletaTraIzqPezCafe.obj");
	 ColaPezCafe = Model();
	 ColaPezCafe.LoadModel("Models/fish/Cafe/ColaPezCafe.obj");
	 CuerpoPezCafe = Model();
	 CuerpoPezCafe.LoadModel("Models/fish/Cafe/CuerpoPezCafe.obj");

	//Pez Azul//
	 AletaDerPezAzul = Model();
	 AletaDerPezAzul.LoadModel("Models/fish/Azul/AletaDerPezAzul.obj");
	 AletaIzqPezAzul = Model();
	 AletaIzqPezAzul.LoadModel("Models/fish/Azul/AletaIzqPezAzul.obj");
	 ColaPezAzul = Model();
	 ColaPezAzul.LoadModel("Models/fish/Azul/ColaPezAzul.obj");
	 CuerpoPezAzul = Model();
	 CuerpoPezAzul.LoadModel("Models/fish/Azul/CuerpoPezAzul.obj");

	//Delfin//
	 AletaDerDelfin = Model();
	 AletaDerDelfin.LoadModel("Models/dolphin/AletaDerDelfin.obj");
	 AletaIzqDelfin = Model();
	 AletaIzqDelfin.LoadModel("Models/dolphin/AletaIzqDelfin.obj");
	 ColaDelfin = Model();
	 ColaDelfin.LoadModel("Models/dolphin/ColaDelfin.obj");
	 CuerpoDelDelfin = Model();
	 CuerpoDelDelfin.LoadModel("Models/dolphin/CuerpoDelDelfin.obj");
	 CuerpoTraDelfin = Model();
	 CuerpoTraDelfin.LoadModel("Models/dolphin/CuerpoTraDelfin.obj");	

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	// Lampara calle 1
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		10000.0f, 10000.0f,
		-167.0f, 130.0f, -189.0f,
		2.0f, 2.0f, 1.0f);
	pointLightCount++;

	// Lampara calle 2
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		10000.0f, 10000.0f,
		-167.0f, 100.0f, 120.0f,
		2.0f, 2.0f, 1.0f);
	pointLightCount++;

	// Lampara calle 3
	pointLights[2] = PointLight(1.0f, 1.0f, 1.0f,
		15000.0f, 10000.0f,
		237.0f, 100.0f, -30.0f,
		2.0f, 2.0f, 1.0f);
	pointLightCount++;

	unsigned int spotLightCount = 0;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	//KEYFRAMES DECLARADOS INICIALES
	KeyFrame[0].movAleFrontDerT = 0.0f;
	KeyFrame[0].movAleFrontIzqT = 0.0f;
	KeyFrame[0].movAleTrasDerT = 0.0f;
	KeyFrame[0].movAleTrasIzqT = 0.0f;
	KeyFrame[0].movBodyT = 0.0f;
	KeyFrame[0].rotBodyT = 0;

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0); //modelos
		glm::mat4 modelaux(1.0); //pivotes

		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

		//modelo de piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		shaderList[0].SetDirectionalLight(&mainLight);

		/* -------------------------------------------------------------------------------------------------------------------------
		 Acuario
		 ------------------------------------------------------------------------------------------------------------------------- */
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		acuario2.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// AcuarioPinguino
		// ------------------------------------------------------------------------------------------------------------------------- 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		estPingu.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// AcuarioTortuga
		// ------------------------------------------------------------------------------------------------------------------------- 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		AcuarioTortuga.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// AcuarioTortuga
		// ------------------------------------------------------------------------------------------------------------------------- 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		AcuarioPez.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// AcuarioOrca
		// ------------------------------------------------------------------------------------------------------------------------- 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, 0.4f, -290.0f));
		model = glm::scale(model, glm::vec3(4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		AcuarioExt.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// AcuarioManta
		// ------------------------------------------------------------------------------------------------------------------------- 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, 0.4f, -290.0f));
		model = glm::scale(model, glm::vec3(4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		AcuarioExt.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// AcuarioTiburon
		// ------------------------------------------------------------------------------------------------------------------------- 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, 0.6f, -175.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		AcuarioExt.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// AcuarioDelfin
		// ------------------------------------------------------------------------------------------------------------------------- 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, 0.6f, -175.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		AcuarioExt.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// Marco1
		// ------------------------------------------------------------------------------------------------------------------------- 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		marco1.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// Marco2
		// ------------------------------------------------------------------------------------------------------------------------- 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		marco2.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// Marco3
		// ------------------------------------------------------------------------------------------------------------------------- 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		marco3.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// reja
		// ------------------------------------------------------------------------------------------------------------------------- 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		reja.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// camino
		// ------------------------------------------------------------------------------------------------------------------------- 
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		camino.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// Orca
		// ------------------------------------------------------------------------------------------------------------------------- 

		//CuerpoPrincipal
		model = glm::mat4(1.0);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(78.0f, 10.0f, -285.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoDelOrca.RenderModel();

		//CuerpoTrasero
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoTraOrca.RenderModel();

		//AletaIzq
		model = glm::mat4(1.0);
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaIzqOrca.RenderModel();

		//AletaDer
		model = glm::mat4(1.0);
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaDerOrca.RenderModel();

		//Cola
		model = glm::mat4(1.0);
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ColaOrca.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// Tortuga
		// ------------------------------------------------------------------------------------------------------------------------- 

		//Torso
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-82.0f, 10.0f, -26.0f));
		//model = glm::scale(model, glm::vec3(0.12f));
		model = glm::rotate(model, rotBody * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(movBody, 0.0f, .0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		modelaux = model;
		TorsoTortuga.RenderModel();

		//cabeza
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		CabezaTortuga.RenderModel();

		//AletaFronIzq
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, .0f));
		model = glm::rotate(model, movAleFrontIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaIzqTortuga.RenderModel();

		//AletaFronDer
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, .0f));
		model = glm::rotate(model, movAleFrontDer * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaDerTortuga.RenderModel();

		//AletaIzqTra
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, .0f));
		model = glm::rotate(model, movAleTrasIzq * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaTraIzqTortuga.RenderModel();

		//AletaDerTra
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, .0f));
		model = glm::rotate(model, movAleTrasDer * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaTraDerTortuga.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// Tiburon
		// ------------------------------------------------------------------------------------------------------------------------- 

		//CuerpoPrincipal
		model = glm::translate(glm::mat4(1.0f), glm::vec3(78.0f , 8.0f, -175.0f));
		model = glm::scale(model, glm::vec3(0.55f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoPrinTiburon.RenderModel();

		//CuerpoDelantero

		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoDelTiburon.RenderModel();

		//AletaIzq
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaIzqTiburon.RenderModel();

		//AletaDer
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaDerTiburon.RenderModel();

		//Cola
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ColaTiburon.RenderModel();

		//Boca
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BocaTiburon.RenderModel();;


		// -------------------------------------------------------------------------------------------------------------------------
		// Pinguino
		// ------------------------------------------------------------------------------------------------------------------------- 

		//CuerpoPrincipal
		model = glm::translate(glm::mat4(1.0f), glm::vec3(42.0f, 1.0f, -75.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		TorsoPinguino.RenderModel();

		//AletaDer

		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaDerPinguino.RenderModel();

		//AletaIzq
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaIzqPinguino.RenderModel();

		//Cabeza
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CabezaPinguino.RenderModel();

		//PieDer
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PieDerPinguino.RenderModel();

		//PieIzq
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PieIzqPinguino.RenderModel();
	
		// -------------------------------------------------------------------------------------------------------------------------
		// Manta
		// ------------------------------------------------------------------------------------------------------------------------- 

		//CuerpoPrincipal
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-43.0f, 6.0f, -295.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		CuerpoManta.RenderModel();

		////AletaSupIzq
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaSupIzqManta.RenderModel();

		//AletaSupDer
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaSupDerManta.RenderModel();

		//AletaInfDer
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaInfDerManta.RenderModel();

		//AletaInfIzq
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaInfIzqManta.RenderModel();

		//Cola
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ColaManta.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// Pez Cafe
		// ------------------------------------------------------------------------------------------------------------------------- 

		//CuerpoPrincipal
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 12.0f, 60.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoPezCafe.RenderModel();

		////AletaDelDer

		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaDelDerPezCafe.RenderModel();

		//AletaDelIzq
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaDelIzqPezCafe.RenderModel();

		//AletaTraDer
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaTraDerPezCafe.RenderModel();

		//AletaTraIzq
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaTraIzqPezCafe.RenderModel();

		//Cola
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ColaPezCafe.RenderModel();


		// -------------------------------------------------------------------------------------------------------------------------
		// Pez Azul
		// ------------------------------------------------------------------------------------------------------------------------- 

		//CuerpoPrincipal
		modelaux = model = glm::translate(glm::mat4(1.0f), glm::vec3(48.0f, 12.0f, 60.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoPezAzul.RenderModel();

		////AletaDer
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaDerPezAzul.RenderModel();

		//AletaIzq
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaIzqPezAzul.RenderModel();

		//Cola
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ColaPezAzul.RenderModel();


		// -------------------------------------------------------------------------------------------------------------------------
		// Delfin
		// ------------------------------------------------------------------------------------------------------------------------- 

		//CuerpoPrincipal
		model = modelaux = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, 5.0f, -180.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoDelDelfin.RenderModel();

		////AletaDer
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaDerDelfin.RenderModel();

		//AletaIzq
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AletaIzqDelfin.RenderModel();

		//Cola
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ColaDelfin.RenderModel();

		//CuerpoTrasero
		model = glm::translate(modelaux, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoTraDelfin.RenderModel();

		//Acá todo lo de GL_BLEND (Texturas)
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		////Textura de anuncio en pantalla
		//toffsetu += 0.0001;
		//if (toffsetu > 1.0) toffsetu = 0.0;
		//toffset = glm::vec2(toffsetu, 0.0f);
		//model = glm::mat4(1.0);
		//model = glm::scale(model, glm::vec3(200.0f, 1.0f, 50.0f));
		////model = glm::translate(model, glm::vec3(200.0f, 100.0f, 50.0f));
		////model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 0.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////AnuncioTexture.UseTexture();
		//meshList[3]->RenderMesh();
		//glDisable(GL_BLEND);

		//HELPER PARA COLOCAR OBJETOS
		if (delay_helper >= 50.0f) {

			//printf("\nEl objeto está en X: %f , Y: %f , Z: %f", mainWindow.getposx_bh(), mainWindow.getelevacion_bh(), mainWindow.getposz_bh());
			delay_helper = 0.0f;
		}
		delay_helper += deltaTime;

		// SECCION PARA ANIMACIÓN



		// Ciclado Dia-Noche de la luz direccional
		if (delay_daynight >= 360.0f && day_flag == true) {

			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				light_changing, light_changing,
				0.0f, 0.0f, -1.0f);
			shaderList[0].SetDirectionalLight(&mainLight);
			light_changing -= 0.1;
			delay_daynight = 0.0f;

			nowSkybox = { skyboxFaces.begin() + indexSkybox, skyboxFaces.begin() + indexSkybox + 6 };
			skybox = Skybox(nowSkybox);
			indexSkybox += 6;

			if (light_changing <= 0.3)
				shaderList[0].SetPointLights(pointLights, pointLightCount);

			if (light_changing <= 0.1)
				day_flag = false;

		}

		if (delay_daynight >= 360.0f && day_flag == false) {

			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				light_changing, light_changing,
				0.0f, 0.0f, -1.0f);
			shaderList[0].SetDirectionalLight(&mainLight);
			light_changing += 0.1;
			delay_daynight = 0.0f;

			nowSkybox = { skyboxFaces.begin() + indexSkybox, skyboxFaces.begin() + indexSkybox + 6 };
			skybox = Skybox(nowSkybox);
			indexSkybox -= 6;

			if (light_changing >= 0.4)
				shaderList[0].SetPointLights(pointLights, 0);

			if (light_changing >= 1.0f)
				day_flag = true;
		}

		delay_daynight += deltaTime;

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}