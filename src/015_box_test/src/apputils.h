
#ifndef __appstate_h__
#define __appstate_h__

#include <cstdint>
#include <iostream>
#include <string>

#include <source_location>

#define GLEW_STATIC
#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glu.h>

#include "fmt/format.h"

class CAppState {
    public:
        int32_t appWindowWidth;
        int32_t appWindowHeight;
        float appWindowAspect;
        std::string appName;

        CAppState() {
            appWindowWidth = 1152;
            appWindowHeight = 864;
            appWindowAspect
              = static_cast<float>(appWindowWidth) / static_cast<float>(appWindowHeight);
            appName = "004_fullhouse2";
        };

        // Инициализация параметров окна из JSON
        CAppState(const std::string &configFileName) {
        }

        ~CAppState(){};

        void showCurrentAppState();

    private:
};

GLenum checkOpenGLError(const std::source_location lctn = std::source_location::current());

#endif