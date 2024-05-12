
#include <GL/glx.h>
#include <string>
#include <iostream>
#include <format>
#include <print>

#include "config/Config.h"
#include "Render.h"
#include "RenderLegacyGL.h"

#include "spdlog/spdlog.h"

namespace tire {

RenderLegacyGL::RenderLegacyGL() : Render{} {
    configureGl();
    setSwapInterval(1);
}

RenderLegacyGL::~RenderLegacyGL() {
    glXDestroyContext(display_, glContext_);
}

void RenderLegacyGL::configureGl() {
    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(&MessageCallback, nullptr);

    __detail_tire::ctxErrorOccurred = false;
    int (*oldHandler)(Display *, XErrorEvent *) = XSetErrorHandler(&__detail_tire::ctxErrorHandler);

    std::array<int, 5> context_attribs;
    context_attribs[0] = GLX_CONTEXT_MAJOR_VERSION_ARB;
    context_attribs[1] = 1;
    context_attribs[2] = GLX_CONTEXT_MINOR_VERSION_ARB;
    context_attribs[3] = 0;
    // context_attribs[4] = GLX_CONTEXT_PROFILE_MASK_ARB;
    // context_attribs[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
    context_attribs[4] = None;

    glContext_
      = glXCreateContextAttribsARB(display_, bestFbc_, nullptr, True, context_attribs.data());

    if (__detail_tire::ctxErrorOccurred || !glContext_) {
        throw std::runtime_error("failed to create an OpenGL context");
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

    vendor_ = (const char *)glGetString(GL_VENDOR);
    renderer_ = (const char *)glGetString(GL_RENDERER);
    glVersion_ = (const char *)glGetString(GL_VERSION);
    glslVersion_ = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

void RenderLegacyGL::displayRenderInfo() {
    spdlog::info(
      "OpenGL context info\nvendor - {}\nrenderer - {}\nOpenGL version - {}\nGLSL version - {}",
      vendor_,
      renderer_,
      glVersion_,
      glslVersion_);
}

void RenderLegacyGL::preFrame() {
    glClearColor(0.5f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderLegacyGL::postFrame() {
}

void RenderLegacyGL::setupDebugMessages() {
}

void RenderLegacyGL::swapBuffers() {
    glXSwapBuffers(display_, window_);
};

void RenderLegacyGL::appendToRenderList(std::shared_ptr<tire::Node<point_scalar_type>> node) {
    renderList_.push_back(node);
}

}  // namespace tire
