
module;

#include <string>
#include <iostream>
#include <format>
#include <print>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

export module screen:RenderGL;

import :Render;

namespace tire {
export struct __gl_Render : Render {
        __gl_Render() = default;
        __gl_Render(const __gl_Render& rhs) = delete;
        __gl_Render(__gl_Render&& ths) = delete;

        __gl_Render& operator=(const __gl_Render& rhs) = delete;
        __gl_Render& operator=(__gl_Render&& rhs) = delete;

        ~__gl_Render() override = default;

        void displayRenderInfo() override {
            std::print("vendor - {}\nrenderer - {}\nOpenGL version - {}\nGLSL version - {}\n",
                       vendor_,
                       renderer_,
                       glVersion_,
                       glslVersion_);
        }

    protected:
        std::string vendor_;
        std::string renderer_;
        std::string glVersion_;
        std::string glslVersion_;
};

export struct __glfw_gl_Render : __gl_Render {
        __glfw_gl_Render(GLFWwindow* window = nullptr) : window_{ window } {
            if (!window_) {
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
            glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

            glfwMakeContextCurrent(window_);

            glewExperimental = GL_TRUE;

            if (glewInit() != GLEW_OK) {
                std::cout << "mainApp_glfw_c::init_app(): Failed to initialize GLEW" << std::endl;
                exit(1);
            };

            // Выключаем вертикальную синхронизацию (VSYNC)
            glfwSwapInterval(0);

            vendor_ = (const char*)glGetString(GL_VENDOR);
            renderer_ = (const char*)glGetString(GL_RENDERER);
            glVersion_ = (const char*)glGetString(GL_VERSION);
            glslVersion_ = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
        };

        __glfw_gl_Render(const __glfw_gl_Render& rhs) = delete;
        __glfw_gl_Render(__glfw_gl_Render&& ths) = delete;

        __glfw_gl_Render& operator=(const __glfw_gl_Render& rhs) = delete;
        __glfw_gl_Render& operator=(__glfw_gl_Render&& rhs) = delete;

        ~__glfw_gl_Render() override = default;

    private:
        GLFWwindow* window_;
};

export struct __sdl_gl_Render : __gl_Render {
        __sdl_gl_Render() = default;
        __sdl_gl_Render(const __sdl_gl_Render& rhs) = delete;
        __sdl_gl_Render(__sdl_gl_Render&& ths) = delete;

        __sdl_gl_Render& operator=(const __sdl_gl_Render& rhs) = delete;
        __sdl_gl_Render& operator=(__sdl_gl_Render&& rhs) = delete;

        ~__sdl_gl_Render() override = default;
};

}  // namespace tire
