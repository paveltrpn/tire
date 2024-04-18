
#include <iostream>
#include <format>
#include <memory>
#include <print>

#include "X11Screen.h"
#include "render/RenderGL.h"
#include "render/RenderVK.h"

namespace tire {

X11Screen::X11Screen(const Config& config) : Screen{ config } {
}

X11Screen::~X11Screen() {
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
    return true;
}

void X11Screen::preFrame() {
}

void X11Screen::postFrame() {
}

void X11Screen::initOpenGL(const tire::Config& config) {
    render_ = std::make_unique<__x11_gl_Render>(config);
};

void X11Screen::initVulkan(const tire::Config& config) {
    render_ = std::make_unique<__x11_vk_Render>(config);
};
}  // namespace tire
