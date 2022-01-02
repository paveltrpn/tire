
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "GLFW/glfw3.h"

#include "service.h"

void setup(const AppState_s *appState) {
    glViewport(0, 0, appState->wndWidth, appState->wndHeight);

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f); 
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

int main(int argc, char **argv) {
    AppState_s appState = {.wndWidth = 1152, .wndHeight = 768, .appName = "020_clang"};

    initGlfwWindow(&appState); 

    printf("%s\n", appState.glfwVersionStr);
    printf("%s\n", appState.glVersionStr);
    printf("%s\n", appState.glRenderStr);
    printf("%s\n", appState.glslVersionStr);

    registerGlfwCallbacks(&appState);

    setup(&appState);

    while (!glfwWindowShouldClose(appState.glfwWndPtr)) {
		glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(appState.glfwWndPtr);
	}

    return 0;
}