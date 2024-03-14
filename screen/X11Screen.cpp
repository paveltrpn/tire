
#include <iostream>
#include <format>
#include <print>

#include "X11Screen.h"

namespace tire {

X11Screen::X11Screen(const Config& config) : Screen{ config } {
}

void X11Screen::displayScreenInfo() {
    std::print("X11 based screen\n"
               "=================\n");
    std::print("not implemented\n");
};

void X11Screen::initRender(RenderType renderType, const Config& config) {
    switch (renderType) {
    case RenderType::OPENGL: {
        initOpenGL(config);
        break;
    }
    case RenderType::VULKAN: {
        initVulkan(config);
        break;
    }
    default:
        break;
    };
};

bool X11Screen::isRun() {
    return false;
}

void X11Screen::preFrame() {
}

void X11Screen::postFrame() {
}

void X11Screen::initOpenGL(const tire::Config& config) {};

void X11Screen::initVulkan(const tire::Config& config) {};
}  // namespace tire
