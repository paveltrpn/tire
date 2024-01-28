
module;

#include <iostream>
#include <format>
#include <print>

export module screen:X11Screen;

import :Screen;
import :Render;
import :RenderX11;

namespace tire {

export struct X11Screen final : Screen {
        void init(RenderType renderType) override {
            std::print("X11Screen(): call init()\n");

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
