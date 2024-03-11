
module;

#include <iostream>
#include <format>
#include <print>

#include "Config.h"
#include "Render.h"

export module screen:X11Screen;

import :Screen;

namespace tire {

export struct X11Screen final : Screen {
        X11Screen(const tire::Config& config) : Screen{ config } {
        }

        void displayScreenInfo() override {
            std::print("X11 based screen\n"
                       "=================\n");
            std::print("not implemented\n");
        };

        void initRender(RenderType renderType, const tire::Config& config) override {
            switch (renderType) {
            case RenderType::OPENGL: {
                initOpenGL(config);
                break;
            }
            case RenderType::VULKAN: {
                initVulkan(config);
                break;
            }
            default:
                break;
            };
        };

        bool isRun() override {
            return false;
        }

        void preFrame() override {
        }

        void postFrame() override {
        }

    private:
        void initOpenGL(const tire::Config& config) {};

        void initVulkan(const tire::Config& config) {};
};
}  // namespace tire
