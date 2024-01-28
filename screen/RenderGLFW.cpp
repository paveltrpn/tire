
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

export module screen:RenderGLFW;

import :Render;
import :RenderGL;
import :RenderVK;
import :RenderSF;

namespace tire {

// ======================================================================================
// =============== OpenGL with GLFW initialization struct ===============================
// ======================================================================================
struct __glfw_gl_Render : __gl_Render {
        __glfw_gl_Render(GLFWwindow* window = nullptr) : window_{ window } {
            if (!window_) {
                std::print("GLFW window pointer can't be null!\n");
                std::exit(1);
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
            glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

            glfwMakeContextCurrent(window_);

            glewExperimental = GL_TRUE;

            if (glewInit() != GLEW_OK) {
                std::print("failed to initialize GLEW\n");
                std::exit(1);
            };

            // Выключаем вертикальную синхронизацию (VSYNC)
            glfwSwapInterval(0);

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
struct __glfw_vk_Render : __vk_Render {
        void swapBuffers() override{};
};

export struct __glfw_sf_Render : __sf_Render {};

}  // namespace tire
