
module;

#include <iostream>
#include <format>
#include <print>

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

export module screen:X11Screen;

import :Screen;
import :Render;
import :RenderX11;

namespace tire {

export struct X11Screen final : Screen {
        X11Screen(nlohmann::json& config) : Screen{ config } {
        }

        void displayScreenInfo() override {
            std::print("X11 based screen\n"
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
            default:
                break;
            };
        };

        void run() override {};

    private:
        void initOpenGL() {};

        void initVulkan() {};
};
}  // namespace tire
