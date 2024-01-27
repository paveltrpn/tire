
module;

#include <iostream>
#include <format>
#include <print>

export module screen:X11Screen;

import :Screen;
import :Render;
import :RenderGL;
import :RenderVK;
import :RenderSF;

namespace tire {
export struct X11Screen final : Screen {
        X11Screen() = default;
        X11Screen(const X11Screen& rhs) = delete;
        X11Screen(X11Screen&& ths) = delete;

        X11Screen& operator=(const X11Screen& rhs) = delete;
        X11Screen& operator=(X11Screen&& rhs) = delete;

        ~X11Screen() override {
            delete render_;
        };

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

        void run() override {
        }

    private:
        void initOpenGL() {
        }

        void initVulkan() {
        }

        void initSoftware() {
        }
};
}  // namespace tire
