
#include <iostream>
#include <fmt/format.h>

#include "service.h"

void setup(const SAppState &appSatete) {
    glViewport(0, 0, appSatete.wndWidth, appSatete.wndHeight);

	glClearColor(0.0f, 0.1f, 0.0f, 0.0f); 
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
};

int main(int argc, char** argv) {
    SAppState appSate;
    
    initGlfwWindow(appSate);

    // std::cout << g_appSate.glfwVersionStr << std::endl;
    // std::cout << g_appSate.glVersionStr << std::endl;
    // std::cout << g_appSate.glRenderStr << std::endl;
    // std::cout << g_appSate.glslVersionStr << std::endl;

    registerGlfwCallbacks(appSate);

    setup(appSate);

    while(!glfwWindowShouldClose(appSate.glfwWndPtr)) {
		glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if (appSate.KEY_ESCAPE) {
            glfwSetWindowShouldClose(appSate.glfwWndPtr, GLFW_TRUE);
        }

		glfwSwapBuffers(appSate.glfwWndPtr);
	}

    return 0;
}