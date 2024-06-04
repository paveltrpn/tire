
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

import algebra;
import camera;

namespace tire {

RenderGL::RenderGL() : Render{} {
    configureGl();
    initOpenGLFunctions();
    // setup VSYNC
    setSwapInterval(1);
}

RenderGL::~RenderGL() {
    glXDestroyContext(display_, glContext_);
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

    vendor_ = (const char *)glGetString(GL_VENDOR);
    renderer_ = (const char *)glGetString(GL_RENDERER);
    glVersion_ = (const char *)glGetString(GL_VERSION);
    glslVersion_ = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

void RenderGL::displayRenderInfo() {
    spdlog::info(
      "OpenGL context info\nvendor - {}\nrenderer - {}\nOpenGL version - {}\nGLSL version - {}",
      vendor_,
      renderer_,
      glVersion_,
      glslVersion_);
}

void RenderGL::initMainLoop() {
    // linkProgram();
    program_.link({ { GL_VERTEX_SHADER, shaderSourcesManager_.getVertexShader("basic_gl") },
                    { GL_FRAGMENT_SHADER, shaderSourcesManager_.getFragmentShader("basic_gl") } });

    program_.use();

    // glUniformMatrix4fv(matrix, 1, GL_FALSE, result.data());

    // auto camera = tire::camera::Perspective(
    // 50.0f, static_cast<float>(width_) / static_cast<float>(height_), 0.1f, 100.0f);

    // camera.move(tire::algebra::Vector3f{ 8.0f, 0.0f, -20.0f });
    // camera.rotate(0.0f, 20.0f, 0.0f);

    auto matrix = program_.getUniformLocation("matrix");
    program_.setMatrixUniform(matrix, GL_FALSE, camera_->getMatrix());
    auto color = program_.getUniformLocation("color");
    program_.setVectorUniform(color, tire::algebra::Vector3f{ 0.9f, 0.2f, 0.5f });
};

void RenderGL::preFrame() {
    glViewport(0, 0, width_, height_);
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

void RenderGL::traverse() {
    glEnableVertexAttribArray(0);
    glBindVertexArray(vertexObject_);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glDisableVertexAttribArray(0);
};

}  // namespace tire
