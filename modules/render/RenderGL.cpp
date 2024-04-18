
#include <string>
#include <iostream>
#include <format>
#include <print>

#include "config/Config.h"
#include "Render.h"
#include "RenderGL.h"

#include "spdlog/spdlog.h"

namespace tire {

#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092

typedef GLXContext (
  *glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

RenderGL::RenderGL(const tire::Config& config) : Render{ config } {
    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(&MessageCallback, nullptr);

    // Get the default screen's GLX extension list
    const char* glxExts = glXQueryExtensionsString(display_, DefaultScreen(display_));

    // NOTE: It is not necessary to create or make current to a context before
    // calling glXGetProcAddressARB
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB(
      (const GLubyte*)"glXCreateContextAttribsARB");

    GLXContext ctx = 0;

    // Install an X error handler so the application won't exit if GL 3.0
    // context allocation fails.
    //
    // Note this error handler is global.  All display connections in all threads
    // of a process use the same error handler, so be sure to guard against other
    // threads issuing X commands while this code is running.
    __detail::ctxErrorOccurred = false;
    int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&__detail::ctxErrorHandler);

    // Check for the GLX_ARB_create_context extension string and the function.
    // If either is not present, use GLX 1.3 context creation method.
    if (!__detail::isExtensionSupported(glxExts, "GLX_ARB_create_context")
        || !glXCreateContextAttribsARB) {
        printf("glXCreateContextAttribsARB() not found"
               " ... using old-style GLX context\n");
        ctx = glXCreateNewContext(display_, bestFbc_, GLX_RGBA_TYPE, 0, True);
    }

    // If it does, try to get a GL 3.0 context!
    else {
        int context_attribs[]
          = { GLX_CONTEXT_MAJOR_VERSION_ARB,
              3,
              GLX_CONTEXT_MINOR_VERSION_ARB,
              0,
              // GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
              None };

        spdlog::info("creating context");
        ctx = glXCreateContextAttribsARB(display_, bestFbc_, 0, True, context_attribs);

        // Sync to ensure any errors generated are processed.
        XSync(display_, False);
        if (!__detail::ctxErrorOccurred && ctx)
            printf("Created GL 3.0 context\n");
        else {
            // Couldn't create GL 3.0 context.  Fall back to old-style 2.x context.
            // When a context version below 3.0 is requested, implementations will
            // return the newest context version compatible with OpenGL versions less
            // than version 3.0.
            // GLX_CONTEXT_MAJOR_VERSION_ARB = 1
            context_attribs[1] = 1;
            // GLX_CONTEXT_MINOR_VERSION_ARB = 0
            context_attribs[3] = 0;

            __detail::ctxErrorOccurred = false;

            spdlog::info("Failed to create GL 3.0 context... using old-style GLX context");
            ctx = glXCreateContextAttribsARB(display_, bestFbc_, 0, True, context_attribs);
        }
    }

    // Sync to ensure any errors generated are processed.
    XSync(display_, False);

    // Restore the original error handler
    XSetErrorHandler(oldHandler);

    if (__detail::ctxErrorOccurred || !ctx) {
        std::runtime_error("failed to create an OpenGL context");
    }

    // Verifying that context is a direct context
    if (!glXIsDirect(display_, ctx)) {
        spdlog::info("Indirect GLX rendering context obtained");
    } else {
        spdlog::info("Direct GLX rendering context obtained");
    }

    spdlog::info("Making context current");
    glXMakeCurrent(display_, window_, ctx);

    vendor_ = (const char*)glGetString(GL_VENDOR);
    renderer_ = (const char*)glGetString(GL_RENDERER);
    glVersion_ = (const char*)glGetString(GL_VERSION);
    glslVersion_ = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

RenderGL::~RenderGL() {
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
