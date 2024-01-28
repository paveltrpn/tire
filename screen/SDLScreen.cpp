
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
        void init(RenderType renderType) override {
            std::print("SDLScreen(): call init()\n");
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
