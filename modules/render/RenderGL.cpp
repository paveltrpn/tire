
#include <GL/glx.h>
#include <memory>
#include <string>
#include <iostream>
#include <format>
#include <print>
#include <map>

#include "config/Config.h"
#include "Render.h"
#include "RenderGL.h"

#include "render/GLFunctions.h"
#include "spdlog/spdlog.h"

import algebra;
import camera;

namespace tire {

RenderGL::RenderGL() : Render{} {
    checkGlxVersion();
    initGlxExtensions();
    configureGl();
    initOpenGLFunctions();
    // setup VSYNC
    setSwapInterval(1);
}

RenderGL::~RenderGL() {
    glXDestroyContext(display_, glContext_);
}

void RenderGL::checkGlxVersion() {
    int glx_major, glx_minor;

    // FBConfigs were added in GLX version 1.3.
    if (!glXQueryVersion(display_, &glx_major, &glx_minor) || ((glx_major == 1) && (glx_minor < 3))
        || (glx_major < 1)) {
        throw std::runtime_error("invalid GLX version");
    }

    spdlog::info("glx version: {}.{}", glx_major, glx_minor);
}

void RenderGL::initGlxExtensions() {
    // Get the default screen's GLX extension list
    const char *glxExts = glXQueryExtensionsString(display_, DefaultScreen(display_));

    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB(
      (const GLubyte *)"glXCreateContextAttribsARB");

    if (!isExtensionSupported(glxExts, "GLX_ARB_create_context") || !glXCreateContextAttribsARB) {
        throw std::runtime_error("extension glXCreateContextAttribsARB not supported!");
    }

    glXSwapIntervalEXT
      = (glXSwapIntervalEXTProc)glXGetProcAddressARB((const GLubyte *)"glXSwapIntervalEXT");

    if (!isExtensionSupported(glxExts, "GLX_EXT_swap_control") || !glXSwapIntervalEXT) {
        throw std::runtime_error("extension glXSwapIntervalEXT not supported!");
    }
}

void RenderGL::setSwapInterval(int interval) {
    GLXDrawable drawable = glXGetCurrentDrawable();
    if (drawable) {
        glXSwapIntervalEXT(display_, drawable, interval);
    }
}

void RenderGL::configureGl() {
    auto cptr = Config::instance();

    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(&MessageCallback, nullptr);

    __detail_tire::ctxErrorOccurred = false;
    int (*oldHandler)(Display *, XErrorEvent *) = XSetErrorHandler(&__detail_tire::ctxErrorHandler);

    // Check for the GLX_ARB_create_context extension string and the function.
    // If either is not present, use GLX 1.3 context creation method.
    std::array<int, 5> context_attribs;
    context_attribs[0] = GLX_CONTEXT_MAJOR_VERSION_ARB;
    context_attribs[2] = GLX_CONTEXT_MINOR_VERSION_ARB;
    if (cptr->get<bool>("use_maximum_context_version", true)) {
        // this parameters force X11 to use higher context among the possible
        context_attribs[1] = 3;
        context_attribs[3] = 0;
    } else {
        // or use user defined context version with 3.3 by default
        context_attribs[1] = cptr->get<int>("use_context_version_major", 3);
        context_attribs[3] = cptr->get<int>("use_context_version_minor", 3);
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
}

void RenderGL::setupDebugMessages() {
}

void RenderGL::displayRenderInfo() {
    spdlog::info(
      "OpenGL context info\nvendor - {}\nrenderer - {}\nOpenGL version - {}\nGLSL version - {}",
      getGLVendorString(),
      getGLRendererString(),
      getGLVersionString(),
      getGLSLVendorString());
}

void RenderGL::prepareShaders() {
    auto program = opengl::Shader{};
    program.link(
      { { GL_VERTEX_SHADER, shaderSourcesManager_.getVertexShader("basic_color") },
        { GL_FRAGMENT_SHADER, shaderSourcesManager_.getFragmentShader("basic_color") } });

    program.use();

    auto matrix = program.getUniformLocation("matrix");
    program.setMatrixUniform(matrix, GL_FALSE, camera_->getMatrix());
    auto color = program.getUniformLocation("color");
    program.setVectorUniform(color, tire::algebra::Vector3f{ 0.9f, 0.2f, 0.5f });

    programs_.insert(std::pair{ opengl::ShaderID::BASIC_COLOR, program });
}

void RenderGL::initMainLoop() {
    prepareShaders();

    opengl::Shader basic_color = programs_[opengl::ShaderID::BASIC_COLOR];

    basic_color.use();
};

void RenderGL::preFrame() {
    glViewport(0, 0, width_, height_);
    glClearColor(0, 0.5, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderGL::frame() {
    glEnableVertexAttribArray(0);
    glBindVertexArray(vertexObject_);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glDisableVertexAttribArray(0);
};

void RenderGL::postFrame() {
}

void RenderGL::swapBuffers() {
    glXSwapBuffers(display_, window_);
};

void RenderGL::appendToRenderList(std::shared_ptr<tire::Node<point_scalar_type>> node) {
    renderList_.push_back(node);

    glGenBuffers(1, &bufferObject_);
    glGenVertexArrays(1, &vertexObject_);

    glBindBuffer(GL_ARRAY_BUFFER, bufferObject_);
    glBindVertexArray(vertexObject_);
    glBufferData(GL_ARRAY_BUFFER, node->getVerteciesArraySize(), nullptr, GL_DYNAMIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, bufferObject_);
    glBindVertexArray(vertexObject_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, node->getVerteciesArraySize(), node->getVerteciesData());
    // color attribute
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)(12 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    // texture coordinates attribute
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)(24 * sizeof(float)));
    // glEnableVertexAttribArray(2);
}

}  // namespace tire
