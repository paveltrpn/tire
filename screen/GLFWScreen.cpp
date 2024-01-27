
module;

#include <iostream>
#include <format>
#include <print>
#include <string>

#include <GLFW/glfw3.h>

export module screen:GLFWScreen;

import :Screen;
import :Render;
import :RenderGL;
import :RenderVK;
import :RenderSF;

namespace tire {
export struct GLFWScreen final : Screen {
        GLFWScreen(std::string_view name = "app") : Screen{ name } {
        }

        GLFWScreen(const GLFWScreen& rhs) = delete;
        GLFWScreen(GLFWScreen&& ths) = delete;

        GLFWScreen& operator=(const GLFWScreen& rhs) = delete;
        GLFWScreen& operator=(GLFWScreen&& rhs) = delete;

        ~GLFWScreen() override {
            delete render_;

            glfwDestroyWindow(window_);
            glfwTerminate();
        };

        void init(RenderType renderType) override {
            std::print("GLFWScreen(): call init()\n");

            if (glfwInit() != GLFW_TRUE) {
                std::cout << "glfwInit() return - GLFW_FALSE!"
                          << "\n";
                exit(1);
            }

            auto errorCallback
              = [](int, const char* err_str) { std::print("GLFW Error: {}\n", err_str); };

            glfwSetErrorCallback(errorCallback);

            window_ = glfwCreateWindow(width_, height_, appName_.c_str(), nullptr, nullptr);
            if (window_ == nullptr) {
                std::print("Failed to create GLFW window\n");
                glfwTerminate();
                exit(1);
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

            switch (renderType) {
            case RenderType::OPENGL: {
                initOpenGL(window_);
                break;
            }
            case RenderType::VULKAN: {
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
        };

        void run() override {
            while (!glfwWindowShouldClose(window_)) {
                glfwSwapBuffers(window_);
                glfwPollEvents();
            }
        }

    private:
        void initOpenGL(GLFWwindow* window) {
            render_ = new __glfw_gl_Render{ window };
            render_->displayRenderInfo();
        }

        void initVulkan() {
            // render_ = new RenderVK_GLFW{};
        }

        void initSoftware() {
            // render_ = new RenderSF_GLFW{};
        }

        GLFWwindow* window_;
};
}  // namespace tire
