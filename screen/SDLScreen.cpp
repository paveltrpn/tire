
module;

#include <iostream>
#include <format>
#include <print>

export module screen:SDLScreen;

import :Screen;
import :Render;
import :RenderSDL;

namespace tire {

export struct SDLScreen final : Screen {
        void displayScreenInfo() override {
            std::print("SDL based screen\n"
                       "=================\n");
            std::print("not implemented\n");
        };

        void init(RenderType renderType) override {
            switch (renderType) {
            case RenderType::OPENGL: {
                initOpenGL();
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

        void run() override{};

    private:
        void initOpenGL(){};

        void initVulkan(){};

        void initSoftware(){};
};

}  // namespace tire
