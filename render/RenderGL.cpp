
module;

#include <string>
#include <iostream>
#include <format>
#include <print>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "GLFW/glfw3.h"

export module render:RenderGL;

import config;
import :Render;

namespace tire {
void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam) {
    std::print("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
               (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
               type,
               severity,
               message);
}

struct __gl_Render : Render {
        __gl_Render(const tire::Config& config) : Render{ config } {
            // glEnable(GL_DEBUG_OUTPUT);
            // glDebugMessageCallback(&MessageCallback, nullptr);
        }

        void displayRenderInfo() override {
            std::print("vendor - {}\nrenderer - {}\nOpenGL version - {}\nGLSL version - {}\n",
                       vendor_,
                       renderer_,
                       glVersion_,
                       glslVersion_);
        }

    protected:
        void setupDebugMessages() {
        }

        int ctxtVersionMajorMax_{};
        int ctxtVersionMinorMax_{};

        int ctxtVersionMajorUsed_{};
        int ctxtVersionMinorUsed_{};

        std::string vendor_;
        std::string renderer_;
        std::string glVersion_;
        std::string glslVersion_;
};

// ======================================================================================
// =============== OpenGL with GLFW initialization struct ===============================
// ======================================================================================
export struct __glfw_gl_Render : __gl_Render {
        __glfw_gl_Render(GLFWwindow* window, const tire::Config& config)
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

            glfwMakeContextCurrent(window_);

            glewExperimental = GL_TRUE;

            if (glewInit() != GLEW_OK) {
                throw std::runtime_error("failed to initialize GLEW\n");
            }

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

        void preFrame() override {};

        void postFrame() override {
            glfwSwapBuffers(window_);
        };

    private:
        GLFWwindow* window_;
};

// ======================================================================================
// =============== OpenGL with X11 initialization struct ===============================
// ======================================================================================
struct __x11_gl_Render : __gl_Render {
        __x11_gl_Render(const tire::Config& config) : __gl_Render(config){};

        void displayRenderInfo() override {};
        void preFrame() override {};
        void postFrame() override {};
};

}  // namespace tire
