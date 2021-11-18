
#ifndef __service_h__
#define __service_h__

#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <glfw/glfw3.h>

struct SAppState {
    int     wndWidth { 1152 };
    int     wndHeight { 768 };
    
    std::string     appName { "019" };
    GLFWwindow 		*glfwWndPtr;
    GLFWmonitor		*glfwMonitorPtr; 
    
    std::string     glfwVersionStr;
    std::string     glRenderStr;
    std::string     glVersionStr;
    std::string     glslVersionStr;

    bool            KEY_ESCAPE { false };
};

void initGlfwWindow(SAppState & appState);
void registerGlfwCallbacks(SAppState & appState);

#endif
