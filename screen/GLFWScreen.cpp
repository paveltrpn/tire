
module;

#include <iostream>
#include <format>
#include <print>
#include <string>

#include <GLFW/glfw3.h>

export module screen:GLFWScreen;

import :Screen;
import :Render;
import :RenderGLFW;

namespace tire {

export struct GLFWScreen final : Screen {
        GLFWScreen(std::string_view name = "app") : Screen{ name } {
        }

        GLFWScreen(const GLFWScreen& rhs) = delete;
        GLFWScreen(GLFWScreen&& ths) = delete;

        GLFWScreen& operator=(const GLFWScreen& rhs) = delete;
        GLFWScreen& operator=(GLFWScreen&& rhs) = delete;

        ~GLFWScreen() override {
            glfwDestroyWindow(window_);
            glfwTerminate();
        };

        void init(RenderType renderType) override {
            std::print("GLFWScreen(): call init()\n");

            if (glfwInit() != GLFW_TRUE) {
                std::print("GLFW initialization return - GLFW_FALSE!\n");
                std::exit(1);
            }

            auto errorCallback
              = [](int, const char* err_str) { std::print("GLFW Error: {}\n", err_str); };

            glfwSetErrorCallback(errorCallback);

            switch (renderType) {
            case RenderType::OPENGL: {
                window_ = glfwCreateWindow(width_, height_, appName_.c_str(), nullptr, nullptr);
                if (window_ == nullptr) {
                    std::print("Failed to create GLFW window\n");
                    glfwTerminate();
                    exit(1);
                };

                initOpenGL(window_);
                break;
            }
            case RenderType::VULKAN: {
                glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
                glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

                window_ = glfwCreateWindow(width_, height_, appName_.c_str(), nullptr, nullptr);
                if (window_ == nullptr) {
                    std::print("Failed to create GLFW window\n");
                    glfwTerminate();
                    exit(1);
                };

                initVulkan();
                break;
            }
            case RenderType::SOFTWARE: {
                initSoftware();
                break;
            }
            default:
                break;
            };

            glfwSetWindowPos(window_, posX_, posY_);

            auto keyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                if (key == GLFW_KEY_ESCAPE) {
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                }

                if (key == GLFW_KEY_E && GLFW_PRESS) {
                    std::cout << std::format("Key E pressed\n");
                }
            };

            glfwSetKeyCallback(window_, keyCallback);
        };

        void run() override {
            while (!glfwWindowShouldClose(window_)) {
                render_->swapBuffers();
                glfwPollEvents();
            }
        }

    private:
        void initOpenGL(GLFWwindow* window) {
            render_ = new __glfw_gl_Render{ window };
            render_->displayRenderInfo();
        }

        void initVulkan() {
            render_ = new __glfw_vk_Render{};
            // render_->displayRenderInfo();
        }

        void initSoftware() {
        }

        GLFWwindow* window_;
};

}  // namespace tire
