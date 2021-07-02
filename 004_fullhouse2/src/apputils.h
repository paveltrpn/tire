
#ifndef __appstate_h__
#define __appstate_h__

#include <iostream>
#include <string>
#include <cstdint>
#include <source_location>

#include <GL/gl.h>

#include "fmt/format.h"

class CAppState {
    public:
        int32_t 		appWindowWidth;
        int32_t 		appWindowHeight;
        float 			appWindowAspect;
        std::string 	appName;

        CAppState() {
            appWindowWidth      = 1024;
            appWindowHeight     = 768;
            appWindowAspect     = static_cast<float>(appWindowWidth) / static_cast<float>(appWindowHeight);
            appName             = "004_fullhouse2";
        };

        // Инициализация параметров окна из JSON
        CAppState(const std::string &configFileName) {

        }

        ~CAppState() {};

        void showCurrentAppState();

    private:

};

GLenum checkOpenGLError(const std::source_location lctn);

#endif