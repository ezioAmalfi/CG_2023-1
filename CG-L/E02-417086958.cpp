//Pr�ctica 2 semestre 2023-1: �ndices, mesh, proyecciones, transformaciones geom�tricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al c�digo
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
float angulo = 0.0f;

//Pir�mide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}
//V�rtices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetras()
{
	GLfloat vertices_letras[] = {	
			//X	  Y	   Z	    R	  G		B     I
			 1.0f, -0.6f, 0.5f,   0.0f, 0.0f, 1.0f,
			 0.4f, -0.6f, 0.5f,   0.0f, 0.0f, 1.0f,
			 1.0f, -0.4f, 0.5f,   0.0f, 0.0f, 1.0f,
			 1.0f, -0.4f, 0.5f,   0.0f, 0.0f, 1.0f,
			 0.4f, -0.4, 0.5f,   0.0f, 0.0f, 1.0f,
			 0.4f, -0.6, 0.5f,   0.0f, 0.0f, 1.0f,
			 0.87f, -0.4f, 0.5f,  0.0f, 0.0f, 1.0f,
			 0.87f, 0.1f, 0.5f,   0.0f, 0.0f, 1.0f,
			 0.55f, - 0.4f, 0.5f, 0.0f, 0.0f, 1.0f,
			 0.55f, -0.4f, 0.5f,  0.0f, 0.0f, 1.0f,
			 0.55f, 0.1f, 0.5f,   0.0f, 0.0f, 1.0f,
			 0.87f, 0.1f, 0.5f,   0.0f, 0.0f, 1.0f,			
			 1.0f, 0.1f, 0.5f,    0.0f, 0.0f, 1.0f,
			 0.4f, 0.1f, 0.5f,    0.0f, 0.0f, 1.0f,
			 1.0f, 0.4f, 0.5f,    0.0f, 0.0f, 1.0f,
			 1.0f, 0.4f, 0.5f,    0.0f, 0.0f, 1.0f,
			 0.4f, 0.4f, 0.5f,    0.0f, 0.0f, 1.0f,
			 0.4f, 0.1f, 0.5f,    0.0f, 0.0f, 1.0f,

			//X	    Y	   Z	 R	   G     B     F
			-0.31f, -0.6f, 0.5f, 0.0f, 1.0f, 0.0f,
			-0.08f, -0.6f, 0.5f, 0.0f, 1.0f, 0.0f,
			-0.08f, 0.4f, 0.5f,  0.0f, 1.0f, 0.0f,
			-0.08f, 0.4f, 0.5f,  0.0f, 1.0f, 0.0f,
			-0.31f, 0.4f, 0.5f,  0.0f, 1.0f, 0.0f,
			-0.31f, -0.6f, 0.5f, 0.0f, 1.0f, 0.0f,
			-0.08f, -0.4f, 0.5f, 0.0f, 1.0f, 0.0f,
			0.15f, -0.4f, 0.5f,  0.0f, 1.0f, 0.0f,
			0.15f, -0.1f, 0.5f,  0.0f, 1.0f, 0.0f,
			0.15f, -0.1f, 0.5f,  0.0f, 1.0f, 0.0f,
			-0.08f, -0.1f, 0.5f, 0.0f, 1.0f, 0.0f,
			-0.08f, -0.4f, 0.5f, 0.0f, 1.0f, 0.0f,
			-0.08f, 0.1f, 0.5f,  0.0f, 1.0f, 0.0f,
			-0.08f, 0.4f, 0.5f,  0.0f, 1.0f, 0.0f,
			0.31f, 0.4f, 0.5f,   0.0f, 1.0f, 0.0f,
			0.31f, 0.4f, 0.5f,   0.0f, 1.0f, 0.0f,
			-0.08f, 0.1f, 0.5f,  0.0f, 1.0f, 0.0f,
			0.31f, 0.1f, 0.5f,   0.0f, 1.0f, 0.0f,

			//X	    Y	   Z	 R	   G     B     A
			-1.0f, -0.6f, 0.5f,  1.0f, 0.0f, 0.0f,
			-0.79f, -0.6f, 0.5f, 1.0f, 0.0f, 0.0f,
			-0.71f, 0.4f, 0.5f,  1.0f, 0.0f, 0.0f,
			-0.62f, -0.6f, 0.5f, 1.0f, 0.0f, 0.0f,
			-0.4f, -0.6f, 0.5f,  1.0f, 0.0f, 0.0f,
			-0.71f, 0.4f, 0.5f,  1.0f, 0.0f, 0.0f,
			-0.78f, -0.4f, 0.5f, 1.0f, 0.0f, 0.0f,
			-0.63f, -0.4f, 0.5f,  1.0f, 0.0f, 0.0f,
			-0.61f, -0.3f, 0.5f, 1.0f, 0.0f, 0.0f,
			-0.63f, -0.31f, 0.5f, 1.0f, 0.0f, 0.0f,
			- 0.78f, -0.4f, 0.5f,  1.0f, 0.0f, 0.0f,
			-0.77f, -0.31f, 0.5f, 1.0f, 0.0f, 0.0f
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,294);
	meshColorList.push_back(letras);

}

void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar �ndices: objetos: cubo y  pir�mide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//CreaPiramide(); //�ndice 0 en MeshList
	//CrearCubo();//�ndice 1 en MeshList
	CrearLetras(); //unico que usa MeshColor, �ndice 0 en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		
															
		//Para las letras hay que usar el segundo set de shaders con �ndice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		//Inicializar matriz de dimensi�n 4x4 que servir� como matriz de modelo para almacenar las trasnformaciones geom�tricas
		glm::mat4 model(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		//Para el cubo y la pir�mide se usa el primer set de shaders con �ndice 0 en ShaderList


		//shaderList[0].useShader(); 
		//uniformModel = shaderList[0].getModelLocation();
		//uniformProjection = shaderList[0].getProjectLocation();

		//angulo += 0.01;
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshList[0]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslaci�n
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACI�N //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/