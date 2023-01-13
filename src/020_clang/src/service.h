
#ifndef __service_h__
#define __service_h__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "GLFW/glfw3.h"

typedef struct appState_s {
    int     wndWidth;
    int     wndHeight;
    
    char    *appName;

    GLFWwindow 		*glfwWndPtr;
    GLFWmonitor		*glfwMonitorPtr; 
    
    // 7 байт для строки в формате Х.Х.Х и нуль-терминатор,
    // где Х - один разряд
    char     glfwVersionStr[7];
    char     *glRenderStr;
    char     *glVersionStr;
    char     *glslVersionStr;
} appState_s;

void initGlfwWindow(appState_s *appState);
void registerGlfwCallbacks(appState_s *appState);

#endif