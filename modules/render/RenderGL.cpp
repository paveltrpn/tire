
#include <string>
#include <iostream>
#include <format>
#include <print>

#include <GL/gl.h>
#include <GL/glu.h>

#include "GLFW/glfw3.h"

#include "config/Config.h"
#include "Render.h"
#include "RenderGL.h"

namespace tire {

__gl_Render::__gl_Render(const tire::Config& config) : Render{ config } {
    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(&MessageCallback, nullptr);
}

void __gl_Render::displayRenderInfo() {
    std::print("vendor - {}\nrenderer - {}\nOpenGL version - {}\nGLSL version - {}\n",
               vendor_,
               renderer_,
               glVersion_,
               glslVersion_);
}

void __gl_Render::setupDebugMessages() {
}

// ======================================================================================
// =============== OpenGL with GLFW initialization struct ===============================
// ======================================================================================
__glfw_gl_Render::__glfw_gl_Render(GLFWwindow* window, const tire::Config& config)
    : __gl_Render{ config }, window_{ window } {
    if (!window_) {
        throw std::runtime_error("GLFW window pointer can't be null!\n");
    }

    glGetIntegerv(GLFW_CONTEXT_VERSION_MAJOR, &ctxtVersionMajorMax_);
    glGetIntegerv(GLFW_CONTEXT_VERSION_MINOR, &ctxtVersionMinorMax_);

    if (config_.get<bool>("use_maximum_context_version")) {
        ctxtVersionMajorUsed_ = ctxtVersionMajorMax_;
        ctxtVersionMinorUsed_ = ctxtVersionMinorMax_;
    } else {
        ctxtVersionMajorUsed_ = config_.get<int>("use_context_version_major");
        ctxtVersionMinorUsed_ = config_.get<int>("use_context_version_minor");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ctxtVersionMajorUsed_);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ctxtVersionMinorUsed_);

    if (doublebuffer_) {
        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    } else {
        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
    }

    glfwMakeContextCurrent(window_);

    // vsync off
    if (config_.get<bool>("enable_vsync")) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    vendor_ = (const char*)glGetString(GL_VENDOR);
    renderer_ = (const char*)glGetString(GL_RENDERER);
    glVersion_ = (const char*)glGetString(GL_VERSION);
    glslVersion_ = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
};

void __glfw_gl_Render::preFrame() {};

void __glfw_gl_Render::postFrame() {
    glfwSwapBuffers(window_);
};

// ======================================================================================
// =============== OpenGL with X11 initialization struct ===============================
// ======================================================================================
__x11_gl_Render::__x11_gl_Render(const tire::Config& config) : __gl_Render(config){};

void __x11_gl_Render::displayRenderInfo() {};
void __x11_gl_Render::preFrame() {};
void __x11_gl_Render::postFrame() {};

}  // namespace tire
