
module;

#include <string>
#include <iostream>
#include <format>
#include <print>

#include <cstdlib>
#include <cstring>
#include <vector>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

export module render:RenderGLFW;

import config;

import :Render;
import :RenderGL;
import :RenderVK;

namespace tire {

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

            if (config_.getBool("use_maximum_context_version")) {
                ctxtVersionMajorUsed_ = ctxtVersionMajorMax_;
                ctxtVersionMinorUsed_ = ctxtVersionMinorMax_;
            } else {
                ctxtVersionMajorUsed_ = config_.getInt("use_context_version_major");
                ctxtVersionMinorUsed_ = config_.getInt("use_context_version_minor");
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ctxtVersionMajorUsed_);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ctxtVersionMinorUsed_);

            glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
            glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

            glfwMakeContextCurrent(window_);

            glewExperimental = GL_TRUE;

            if (glewInit() != GLEW_OK) {
                throw std::runtime_error("failed to initialize GLEW\n");
            }

            // vsync off
            if (config_.getBool("enable_vsync")) {
                glfwSwapInterval(1);
            } else {
                glfwSwapInterval(0);
            }

            vendor_ = (const char*)glGetString(GL_VENDOR);
            renderer_ = (const char*)glGetString(GL_RENDERER);
            glVersion_ = (const char*)glGetString(GL_VERSION);
            glslVersion_ = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
        };

        void swapBuffers() override {
            glfwSwapBuffers(window_);
        };

    private:
        GLFWwindow* window_;
};

// ======================================================================================
// =============== Vulkan with GLFW initialization struct ===============================
// ======================================================================================
export struct __glfw_vk_Render : __vk_Render {
        __glfw_vk_Render(const tire::Config& config) : __vk_Render{ config } {};

        void swapBuffers() override {};
};

}  // namespace tire
