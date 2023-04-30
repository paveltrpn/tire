
#ifndef __service_h__
#define __service_h__

#include <string>

#define GLEW_STATIC
#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glu.h>

#include "GLFW/glfw3.h"

struct appState_s {
        int wndWidth{ 1152 };
        int wndHeight{ 768 };

        std::string appName{ "019" };
        GLFWwindow *glfwWndPtr;
        GLFWmonitor *glfwMonitorPtr;

        std::string glfwVersionStr;
        std::string glRenderStr;
        std::string glVersionStr;
        std::string glslVersionStr;

        bool KEY_ESCAPE{ false };
};

void initGlfwWindow(appState_s &appState);
void registerGlfwCallbacks(appState_s &appState);

#endif
