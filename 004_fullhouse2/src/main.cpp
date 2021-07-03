
#include <iostream>
#include <string>
#include <fmt/format.h>
#include <thread>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "apputils.h"
#include "screen_text.h"
#include "mtrx4.h"
#include "vec3.h"
#include "camera.h"
#include "mesh.h"
#include "timing.h"

GLFWwindow 		*g_appWindow;

const GLubyte *oglRenderString;
const GLubyte *oglVersionString;
const GLubyte *oglslVersionString;

CAppState		g_AppState;
CPerspCamera	g_Camera, g_textCamera;
CScreenText		g_screenText;
BasicBody 		g_BodyList;
CTime			g_Timer;

double g_cameraOffsetYaw {};
double g_cameraOffsetRoll {};

double g_curPositionX {};
double g_curPositionY {};
double g_curPositionEnteredX {};
double g_curPositionEnteredY {};

void windowInit() {
	g_AppState = CAppState();
	g_AppState.showCurrentAppState();

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

	g_appWindow = glfwCreateWindow(g_AppState.appWindowWidth, g_AppState.appWindowHeight, g_AppState.appName.c_str(), nullptr, nullptr);
	if (g_appWindow == nullptr) {
		std::cout << "windowInit(): Failed to create GLFW window" << std::endl;
		glfwTerminate();
		std::exit(1);
	};

	glfwMakeContextCurrent(g_appWindow);

	glewExperimental = GL_TRUE;
	
	if (glewInit() != GLEW_OK) {
	    std::cout << "windowInit(): Failed to initialize GLEW" << std::endl;
	    exit(1);
	};

	// Выключаем вертикальную синхронизацию (VSYNC)
	// glfwSwapInterval(0);
	
	oglRenderString = glGetString(GL_RENDERER);
	oglVersionString = glGetString(GL_VERSION);
	oglslVersionString = glGetString(GL_SHADING_LANGUAGE_VERSION);
	
	std::cout << fmt::format("windowInit():\n  oglRenderString - {}\n  oglVersionString - {}\n  oglslVersionString - {}\n", oglRenderString, oglVersionString, oglslVersionString);
}

void registerGLFWCallbacks() {
	auto keyCallback = [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		if (key == GLFW_KEY_W && GLFW_PRESS) {
			g_Camera.moveCamera(vec3(0.0f, 0.0f, 0.1f));
		}

		if (key == GLFW_KEY_S && GLFW_PRESS) {
			g_Camera.moveCamera(vec3(0.0f, 0.0f, -0.1f));
		}
	};
	glfwSetKeyCallback(g_appWindow, keyCallback);

	auto cursorPosCallback = [] (GLFWwindow* window, double posX, double posY) {
		g_curPositionX = posX;
		g_curPositionY = posY;

		g_cameraOffsetYaw = g_curPositionX - g_curPositionEnteredX;
		g_curPositionEnteredX = g_curPositionX;

		g_cameraOffsetRoll = g_curPositionY - g_curPositionEnteredY;
		g_curPositionEnteredY = g_curPositionY;
	};
	glfwSetCursorPosCallback(g_appWindow, cursorPosCallback);

	auto cursorEnterCallback = [] (GLFWwindow* window, int entered) {
		if (entered) {
        	// The cursor entered the content area of the window
			glfwGetCursorPos(window, &g_curPositionEnteredX, &g_curPositionEnteredY);
    	} else {
        	// The cursor left the content area of the window
    	}
	};
	glfwSetCursorEnterCallback(g_appWindow, cursorEnterCallback);
}

void appSetup() {
	windowInit();
	registerGLFWCallbacks();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	g_Timer = CTime();

	g_Camera.setCameraPosition({0.0f, 0.0f, -15.0f});
	g_Camera.updateViewMatrix();

	g_textCamera.setCameraPosition({0.0f, 0.0f, -20.0f});
	g_textCamera.updateViewMatrix();

	g_screenText.loadFont("assets/RobotoMono-2048-1024-64-128.jpg");

	g_BodyList = BasicBody(BasicBody::ICOSAHEDRON, vec3( 0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(3.0f, 3.0f, 3.0f));
}

void appLoop() {
	uint64_t frameBeginTime;
	float frameTime;
	uint32_t frameCount = 0, fps;
	CTimeDelay oneSecondDelay(1000);

	while(!glfwWindowShouldClose(g_appWindow)) {
		frameBeginTime = g_Timer.getMs();
		frameCount++;

		glfwPollEvents();
		
		// -----------------------------------------------------------
		// Отрисовка тел из BodyList
		// -----------------------------------------------------------

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		g_Camera.rotateCamera(0.0f, 0.0f, 0.1f);
		g_Camera.updateViewMatrix();
		glLoadMatrixf(g_Camera.getCmrMatrixPointer());

		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();

		glDisable(GL_TEXTURE_2D);

		g_BodyList.updateAndDraw();

		// -----------------------------------------------------------
		// Отрисовка текста
		// -----------------------------------------------------------

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);  

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(g_textCamera.getCmrMatrixPointer());

		g_screenText.setTextPosition(-10.5f, 8.0f);
		g_screenText.drawString(fmt::format("frame time = {}", frameTime));

		g_screenText.setTextPosition(-10.5f, 7.1f);
		g_screenText.drawString(fmt::format("frames per second = {}", fps));

		g_screenText.setTextPosition(-10.5f, 6.2f);
		g_screenText.drawString(fmt::format("cursorX = {}, cursorY = {}", g_cameraOffsetYaw, g_cameraOffsetRoll));

		// -----------------------------------------------------------

		glfwSwapBuffers(g_appWindow);

		if (oneSecondDelay.isPassed()) {
			fps = frameCount;
			frameCount = 0;
			oneSecondDelay.reset();
		}

		frameTime = static_cast<float>(g_Timer.getMs() - frameBeginTime) / 1000.0f;
		frameBeginTime = 0;
	}
}

void appDefer() {
	glfwDestroyWindow(g_appWindow);
	glfwTerminate();
}

int main(int argc, char *argv[]) {

    appSetup();
    
    appLoop();

    appDefer();

    return 0;
}
