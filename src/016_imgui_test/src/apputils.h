
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
#include <GLFW/glfw3.h>

#include "fmt/format.h"

//На расстоянии Х пикселей от границ окна находятся зоны, в которых курсор двигает
constexpr uint32_t c_moveZoneDst = 32;

//Скорости вращения и перемещения камеры при 60 fps
constexpr float c_cmrMoveSpd = 0.4f;
constexpr float c_cmrRotnSpd = 1.0f;

class CAppState {
    public:
        int32_t appWindowWidth;
        int32_t appWindowHeight;
        float appWindowAspect;
        std::string appName;

        double curPositionX{};
        double curPositionY{};
        bool rightButtonClick{ false };

        CAppState() {
            appWindowWidth = 1152;
            appWindowHeight = 864;
            appWindowAspect
              = static_cast<float>(appWindowWidth) / static_cast<float>(appWindowHeight);
            appName = "004_fullhouse2";
        };

        // Инициализация параметров окна из JSON
        CAppState(const std::string& configFileName) {
        }

        ~CAppState(){};

        CAppState(const CAppState& foo) = delete;
        CAppState(const CAppState&& foo) = delete;
        CAppState operator=(const CAppState& foo) = delete;
        CAppState operator=(const CAppState&& foo) = delete;

        void showCurrentAppState();

    private:
};

GLenum checkOpenGLError(const std::source_location lctn = std::source_location::current());

#endif