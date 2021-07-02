
#include <iostream>
#include <string>
#include <fmt/format.h>
		
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "apputils.h"
#include "mtrx4.h"
#include "vec3.h"
#include "camera.h"
#include "mesh.h"

GLFWwindow 		*appWindow;

const GLubyte *oglRenderString;
const GLubyte *oglVersionString;
const GLubyte *oglslVersionString;

CAppState		gAppState;
CameraPersp		gCamera;
BasicBody 		gBody;

void windowInit() {
	gAppState = CAppState();
	gAppState.showCurrentAppState();

	if (glfwInit() != GLFW_TRUE) {
		std::cout << "windowInit(): glfwInit() return - GLFW_FALSE!" << "\n";
		std::exit(1);
	}

	auto errorCallback = [] (int, const char* err_str) {
		std::cout << "GLFW Error: " << err_str << std::endl;
	};
	glfwSetErrorCallback(errorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	appWindow = glfwCreateWindow(gAppState.appWindowWidth, gAppState.appWindowHeight, gAppState.appName.c_str(), nullptr, nullptr);
	if (appWindow == nullptr) {
		std::cout << "windowInit(): Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(1);
	};

	glfwMakeContextCurrent(appWindow);

	glewExperimental = GL_TRUE;
	
	if (glewInit() != GLEW_OK) {
	    std::cout << "windowInit(): Failed to initialize GLEW" << std::endl;
	    exit(1);
	};

	// Выключаем вертикальную синхронизацию (VSYNC)
	glfwSwapInterval(0);
	
	auto keyCallback = [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		if (key == GLFW_KEY_E && GLFW_PRESS) {
			std::cout << fmt::format("windowInit(): Key E pressed\n");
		}
	};
	glfwSetKeyCallback(appWindow, keyCallback);

	oglRenderString = glGetString(GL_RENDERER);
	oglVersionString = glGetString(GL_VERSION);
	oglslVersionString = glGetString(GL_SHADING_LANGUAGE_VERSION);
	
	std::cout << fmt::format("windowInit():\n  oglRenderString - {}\n  oglVersionString - {}\n  oglslVersionString - {}\n", oglRenderString, oglVersionString, oglslVersionString);
}

void appSetup() {
	windowInit();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	gCamera.setCameraPosition({0.0f, 0.0f, -10.0f});
	gCamera.updatePosition();

	gBody = BasicBody(BasicBody::ICOSAHEDRON, vec3( 0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(3.0f, 3.0f, 3.0f));
}

void appLoop() {
	while(!glfwWindowShouldClose(appWindow)) {
		glfwPollEvents();

   		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		glLoadMatrixf(gCamera.getCmrMatrixPointer());
		//gCamera.updatePosition();

		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();

		// ---------------
		// Отрисовка тел из BodyList
		// ---------------
		gBody.updateAndDraw();
		// ---------------	

		glfwSwapBuffers(appWindow);
	}
}

void appDefer() {
	glfwDestroyWindow(appWindow);
	glfwTerminate();
}

int main(int argc, char *argv[]) {

    appSetup();
    
    appLoop();

    appDefer();

    return 0;
}
