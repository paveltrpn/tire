
#include <iostream>
#include <fmt/format.h>

#include "service.h"
#include "shader.h"

void setup(const SAppState &appState) {
    glViewport(0, 0, appState.wndWidth, appState.wndHeight);

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

    COglProgram foo;

    foo.appendShader({{GL_VERTEX_SHADER, "assets/vs.glsl"}, {GL_FRAGMENT_SHADER, "assets/fs.glsl"}});
    foo.linkProgram();

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