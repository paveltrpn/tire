
#include "service.h"

void initGlfwWindow(appState_s *appState) {
	if (glfwInit() != GLFW_TRUE) {
		printf("initWindow(): glfwInit() return - GLFW_FALSE!\n");
		exit(1);
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE,    GLFW_FALSE); 

	appState->glfwWndPtr = glfwCreateWindow(appState->wndWidth, appState->wndHeight, appState->appName, NULL, NULL);
	if (appState->glfwWndPtr == NULL) {
		printf("initWindow(): Failed to create GLFW window!\n");
		glfwTerminate();
		exit(1);
	};
	    
    glfwMakeContextCurrent(appState->glfwWndPtr);

    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    snprintf(appState->glfwVersionStr, 6, "%i.%i.%i", major, minor, rev);

	glewExperimental = GL_TRUE;
	
	if (glewInit() != GLEW_OK) {
	    printf("initGlfwWindow(): Failed to initialize GLEW\n");
	    exit(1);
	};

	// ВКЛ-ВЫКЛ вертикальную синхронизацию (VSYNC)
	// Лок на 60 фпс
	glfwSwapInterval(true);

    appState->glRenderStr        = glGetString(GL_RENDERER);
	appState->glVersionStr       = glGetString(GL_VERSION);
	appState->glslVersionStr     = glGetString(GL_SHADING_LANGUAGE_VERSION);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_ESCAPE && GLFW_RELEASE) {
        
    }
};

void registerGlfwCallbacks(appState_s *appState) {
	glfwSetKeyCallback(appState->glfwWndPtr, keyCallback);
}
