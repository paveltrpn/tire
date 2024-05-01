
#include <GL/glx.h>
#include <memory>
#include <string>
#include <iostream>
#include <format>
#include <print>

#include "config/Config.h"
#include "Render.h"
#include "RenderGL.h"

#include "render/GLFunctions.h"
#include "spdlog/spdlog.h"

namespace tire {

RenderGL::RenderGL(const tire::Config& config) : Render{ config } {
    configureGl();
    setSwapInterval(1);

    gl = std::make_shared<GLFunctions>();
    gl->initGLFunctions();
}

RenderGL::~RenderGL() {
    glXDestroyContext(display_, glContext_);
}

void RenderGL::configureGl() {
    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(&MessageCallback, nullptr);
    
    __detail_tire::ctxErrorOccurred = false;
    int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&__detail_tire::ctxErrorHandler);

    // Check for the GLX_ARB_create_context extension string and the function.
    // If either is not present, use GLX 1.3 context creation method.
    std::array<int, 5> context_attribs;
    context_attribs[0] = GLX_CONTEXT_MAJOR_VERSION_ARB;
    context_attribs[2] = GLX_CONTEXT_MINOR_VERSION_ARB;
    if (config_.get<bool>("use_maximum_context_version", true)) {
        // this parameters force X11 to use higher context among the possible
        context_attribs[1] = 3;
        context_attribs[3] = 0;
    } else {
        // or use user defined context version
        context_attribs[1] = config_.get<int>("use_context_version_major", 3);
        context_attribs[3] = config_.get<int>("use_context_version_minor", 3);
    }
    context_attribs[4] = GLX_CONTEXT_PROFILE_MASK_ARB;
    context_attribs[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
    context_attribs[6] = None;

    glContext_
      = glXCreateContextAttribsARB(display_, bestFbc_, nullptr, True, context_attribs.data());

    // error ocured
    if (__detail_tire::ctxErrorOccurred && glContext_) {
        throw std::runtime_error("can't create modern OpenGL context!");
    }

    // Sync to ensure any errors generated are processed.
    XSync(display_, False);

    // Restore the original error handler
    XSetErrorHandler(oldHandler);

    if (__detail_tire::ctxErrorOccurred || !glContext_) {
        throw std::runtime_error("failed to create an OpenGL context");
    }

    // Verifying that context is a direct context
    if (!glXIsDirect(display_, glContext_)) {
        spdlog::info("indirect GLX rendering context obtained");
    } else {
        spdlog::info("direct GLX rendering context obtained");
    }

    glXMakeCurrent(display_, window_, glContext_);

    vendor_ = (const char*)glGetString(GL_VENDOR);
    renderer_ = (const char*)glGetString(GL_RENDERER);
    glVersion_ = (const char*)glGetString(GL_VERSION);
    glslVersion_ = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

void RenderGL::displayRenderInfo() {
    spdlog::info(
      "OpenGL context info\nvendor - {}\nrenderer - {}\nOpenGL version - {}\nGLSL version - {}",
      vendor_,
      renderer_,
      glVersion_,
      glslVersion_);
}

void RenderGL::preFrame() {
    glClearColor(0, 0.5, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderGL::postFrame() {
}

void RenderGL::setupDebugMessages() {
}

void RenderGL::swapBuffers() {
    glXSwapBuffers(display_, window_);
};

}  // namespace tire
