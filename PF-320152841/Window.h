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
	//Movimiento de puertas
	GLfloat getmuevex() { return muevex; }
	GLfloat getarticulacion1() { return articulacion1; }

	//Movimiento de puertas
	GLfloat getmuevey() { return muevey; }

	//Abrir o cerrar puertas Principal y corrediza
	GLboolean getAbrirCerrarPuerta() { return abrirCerrarPuerta; }
	GLboolean getAbrirCerrarCuarto() { return abrirCerrarCuarto; }
	GLboolean getAbrirCerrarCorrediza() { return AbrirCerrarCorrediza; }


	//para saber el estado de la luz de la lampara y UFO
	GLboolean getprendida() { return luzprendida; }
	//UFO
	GLboolean getprendidaUFO() { return UFOPrendido; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	//Para el status de la piramide
	bool getCamStatus() { return followCamState; }

	~Window();
	
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	// Movimiento de puertas
	GLfloat muevex;
	GLfloat articulacion1;
	//followCamState
	GLfloat followCamState;

	//Se declara la variable
	GLboolean luzprendida;
	GLboolean UFOPrendido;

	// Abrir o cerrar puertas
	GLfloat muevey;
	GLboolean abrirCerrarPuerta;
	GLboolean abrirCerrarCuarto;
	GLboolean AbrirCerrarCorrediza;

	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

