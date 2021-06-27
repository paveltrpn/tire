
#include <iostream>
#include <string>
#include <fmt/format.h>
#include <source_location>
		
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "mtrx4.h"
#include "vec3.h"
#include "camera.h"
#include "mesh.h"

GLFWwindow 		*appWindow;
int32_t 		appWindowWidth {1024};
int32_t 		appWindowHeight {768};
float 			appWindowAspect = static_cast<float>(appWindowWidth) / static_cast<float>(appWindowHeight);
std::string 	appName {"004_fullhouse2"};

const GLubyte *oglRenderString;
const GLubyte *oglVersionString;
const GLubyte *oglslVersionString;

CameraPersp		gCamera;
BasicBody 		gBody {BasicBody::ICOSAHEDRON, vec3( 0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(3.0f, 3.0f, 3.0f)};

GLenum checkOpenGLError(const std::source_location lctn=std::source_location::current()) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " in file - " << lctn.file_name() << "; line - " << lctn.line() << std::endl;
    }
    return errorCode;
}

void appSetup() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	gCamera.setCameraPosition({0.0f, 0.0f, -10.0f});
	gCamera.updatePosition();
}

void appWindowInit() {
	if (glfwInit() != GLFW_TRUE) {
		std::cout << "doInit(): glfwInit() return - GLFW_FALSE!" << "\n";
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

	appWindow = glfwCreateWindow(appWindowWidth, appWindowHeight, appName.c_str(), nullptr, nullptr);
	if (appWindow == nullptr) {
		std::cout << "doInit(): Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(1);
	};

	glfwMakeContextCurrent(appWindow);

	glewExperimental = GL_TRUE;
	
	if (glewInit() != GLEW_OK) {
	    std::cout << "doInit(): Failed to initialize GLEW" << std::endl;
	    exit(1);
	};

	// Выключаем вертикальную синхронизацию (VSYNC)
	glfwSwapInterval(0);
	
	auto keyCallback = [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		if (key == GLFW_KEY_E && GLFW_PRESS) {
			std::cout << fmt::format("Key E pressed\n");
		}
	};
	glfwSetKeyCallback(appWindow, keyCallback);

	oglRenderString = glGetString(GL_RENDERER);
	oglVersionString = glGetString(GL_VERSION);
	oglslVersionString = glGetString(GL_SHADING_LANGUAGE_VERSION);
	
	std::cout << fmt::format("oglRenderString - {}\noglVersionString - {}\noglslVersionString - {}\n", oglRenderString, oglVersionString, oglslVersionString);
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
    appWindowInit();

    appSetup();
    
    appLoop();

    appDefer();

    return 0;
}
