#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
	public:
		Window();
		Window(GLint windowWidth, GLint windowHeight);
		int Initialise();

		GLfloat getBufferWidth() { return bufferWidth; }
		GLfloat getBufferHeight() { return bufferHeight; }
		GLfloat getXChange();
		GLfloat getYChange();
		GLfloat getmuevex() { return muevex; }

		GLfloat getmuevehely() { return hel_pos_y; }
		GLfloat getmuevehelx() { return hel_pos_x; }

		GLfloat getmuevecarx() { return car_pos_x; }
		GLboolean getcardirx() { return carDir; }

		bool getShouldClose() {
			return  glfwWindowShouldClose(mainWindow);}
		bool* getsKeys() { return keys; }
		void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
		~Window();
	private: 
		GLFWwindow *mainWindow;
		GLint width, height;
		bool keys[1024];
		GLint bufferWidth, bufferHeight;
		void createCallbacks();
		GLfloat lastX;
		GLfloat lastY;
		GLfloat xChange;
		GLfloat yChange;
		GLfloat muevex;

		GLfloat hel_pos_y;
		GLfloat hel_pos_x;

		GLfloat car_pos_x;
		GLboolean carDir;

		bool mouseFirstMoved;
		static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
		static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};