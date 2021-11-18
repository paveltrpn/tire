#include <iostream>

#include "fmt/format.h"
#include "service.h"

void initGlfwWindow(SAppState & appState) {
	if (glfwInit() != GLFW_TRUE) {
		std::cout << fmt::format("initWindow(): glfwInit() return - GLFW_FALSE!\n");
		std::exit(1);
	}

	auto errorCallback = [] (int, const char* err_str) {
		std::cout << "initWindow(): GLFW Error: " << err_str << std::endl;
	};
	glfwSetErrorCallback(errorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE,    GLFW_FALSE); 

	appState.glfwWndPtr = glfwCreateWindow(appState.wndWidth, appState.wndHeight, appState.appName.c_str(), nullptr, nullptr);
	if (appState.glfwWndPtr == nullptr) {
		std::cout << fmt::format("initWindow(): Failed to create GLFW window!\n");
		glfwTerminate();
		std::exit(1);
	};

	glfwMakeContextCurrent(appState.glfwWndPtr);

    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    appState.glfwVersionStr = fmt::format("{}.{}.{}", major, minor, rev);

	glewExperimental = GL_TRUE;
	
	if (glewInit() != GLEW_OK) {
	    std::cout << fmt::format("initWindow(): Failed to initialize GLEW\n");
	    std::exit(1);
	};

	// ВКЛ-ВЫКЛ вертикальную синхронизацию (VSYNC)
	// Лок на 60 фпс
	glfwSwapInterval(true);
	
	appState.glRenderStr        = std::string(reinterpret_cast<char const*>(glGetString(GL_RENDERER)));
	appState.glVersionStr       = std::string(reinterpret_cast<char const*>(glGetString(GL_VERSION)));
	appState.glslVersionStr     = std::string(reinterpret_cast<char const*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
};

void registerGlfwCallbacks(SAppState & appState) {
    // Нужно чтобы отправить appState в static storage duration, иначе лямбда не захватыват appState из аргументов
    // функции, а любое изменение параметров захвата лямбды приводит к тому, что её тип перестаёт совпадать с GLFWkeyfun
    static SAppState &localAppState = appState; 

	auto keyCallback = [] (GLFWwindow* window, int key, int scancode, int action, int mods) mutable {
		if (key == GLFW_KEY_ESCAPE && GLFW_PRESS) {
            localAppState.KEY_ESCAPE = true;
		}

        if (key == GLFW_KEY_ESCAPE && GLFW_RELEASE) {
            localAppState.KEY_ESCAPE = false;
        }
	};
	glfwSetKeyCallback(appState.glfwWndPtr, keyCallback);
}