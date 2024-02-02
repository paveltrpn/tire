
module;

#include <iostream>
#include <format>
#include <print>
#include <string>

#include <GLFW/glfw3.h>

export module screen:GLFWScreen;

import config;
import render;
import :Screen;

namespace tire {

export struct GLFWScreen final : Screen {
        GLFWScreen(const tire::Config& config) : Screen{ config } {
        }

        ~GLFWScreen() override {
            glfwDestroyWindow(window_);
            glfwTerminate();
        };

        void displayScreenInfo() override {
            std::print("GLFW based screen\n"
                       "=================\n");
            std::print("GLFW version: {}\n", glfwVersionString_);
        };

        void initRender(RenderType renderType, const tire::Config& config) override {
            if (glfwInit() != GLFW_TRUE) {
                throw std::runtime_error("GLFW initialization return - GLFW_FALSE!\n");
            }

            glfwVersionString_ = std::string{ glfwGetVersionString() };
            glfwGetVersion(&glfwVersionMajor_, &glfwVersionMinor_, &glfwVersionRev_);

            auto errorCallback
              = [](int, const char* err_str) { std::print("GLFW Error: {}\n", err_str); };

            glfwSetErrorCallback(errorCallback);

            switch (renderType) {
            case RenderType::OPENGL: {
                createWindow();

                initOpenGL(window_, config);
                break;
            }
            case RenderType::VULKAN: {
                glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
                glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

                createWindow();

                initVulkan(config);
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

        bool isRun() override {
            return !glfwWindowShouldClose(window_);
        }

        void preFrame() override {
            glfwPollEvents();
        }

        void postFrame() override {
        }

    private:
        void createWindow() {
            window_ = glfwCreateWindow(width_, height_, appName_.c_str(), nullptr, nullptr);
            if (window_ == nullptr) {
                glfwTerminate();
                throw std::runtime_error("Failed to create GLFW window\n");
            };
        }

        void initOpenGL(GLFWwindow* window, const tire::Config& config) {
            render_ = std::make_unique<tire::__glfw_gl_Render>(window, config);
        }

        void initVulkan(const tire::Config& config) {
            render_ = std::make_unique<tire::__glfw_vk_Render>(config);
        }

        GLFWwindow* window_;

        int glfwVersionMajor_;
        int glfwVersionMinor_;
        int glfwVersionRev_;
        std::string glfwVersionString_;
};

}  // namespace tire
