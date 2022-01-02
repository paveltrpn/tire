
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

typedef struct AppState_s {
    int     wndWidth;
    int     wndHeight;
    
    char    *appName;

    GLFWwindow 		*glfwWndPtr;
    GLFWmonitor		*glfwMonitorPtr; 
    
    // 7 байт для строки в формате Х.Х.Х и нуль-терминатор
    // где Х - цифра
    char     glfwVersionStr[7];
    char     *glRenderStr;
    char     *glVersionStr;
    char     *glslVersionStr;
} AppState_s;

void initGlfwWindow(AppState_s *appState);
void registerGlfwCallbacks(AppState_s *appState);

#endif