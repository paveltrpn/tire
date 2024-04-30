
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

RenderLegacyGL::RenderLegacyGL(const tire::Config& config) : Render{ config } {
    configureGl();
}

RenderLegacyGL::~RenderLegacyGL() {
    glXDestroyContext(display_, glContext_);
}

void RenderLegacyGL::configureGl() {
    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(&MessageCallback, nullptr);

    // Get the default screen's GLX extension list
    const char* glxExts = glXQueryExtensionsString(display_, DefaultScreen(display_));

    // NOTE: It is not necessary to create or make current to a context before
    // calling glXGetProcAddressARB
    using glXCreateContextAttribsARBProc
      = GLXContext (*)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB{ nullptr };
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB(
      (const GLubyte*)"glXCreateContextAttribsARB");

    using glXSwapIntervalEXTProc = void (*)(Display*, GLXDrawable, int);
    glXSwapIntervalEXTProc glXSwapIntervalEXT{ nullptr };
    glXSwapIntervalEXT
      = (glXSwapIntervalEXTProc)glXGetProcAddressARB((const GLubyte*)"glXSwapIntervalEXT");

    GLXDrawable drawable = glXGetCurrentDrawable();
    const int interval = 1;
    if (drawable) {
        glXSwapIntervalEXT(display_, drawable, interval);
    }

    // Install an X error handler so the application won't exit if GL 3.0
    // context allocation fails.
    //
    // Note this error handler is global.  All display connections in all threads
    // of a process use the same error handler, so be sure to guard against other
    // threads issuing X commands while this code is running.
    __detail_tire::ctxErrorOccurred = false;
    int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&__detail_tire::ctxErrorHandler);

    // Check for the GLX_ARB_create_context extension string and the function.
    // If either is not present, use GLX 1.3 context creation method.
    if (!__detail_tire::isExtensionSupported(glxExts, "GLX_ARB_create_context")
        || !glXCreateContextAttribsARB) {
        spdlog::info("glXCreateContextAttribsARB() not found... using old-style GLX context");
        glContext_ = glXCreateNewContext(display_, bestFbc_, GLX_RGBA_TYPE, nullptr, True);
    } else {
        std::array<int, 5> context_attribs;
        context_attribs[0] = GLX_CONTEXT_MAJOR_VERSION_ARB;
        context_attribs[1] = 2;
        context_attribs[2] = GLX_CONTEXT_MINOR_VERSION_ARB;
        context_attribs[3] = 1;
        context_attribs[4] = GLX_CONTEXT_PROFILE_MASK_ARB;
        context_attribs[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
        context_attribs[6] = None;

        glContext_
          = glXCreateContextAttribsARB(display_, bestFbc_, nullptr, True, context_attribs.data());

        // Sync to ensure any errors generated are processed.
        XSync(display_, False);
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

}  // namespace tire
