
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
        X11Screen(const Screen& rhs) = delete;
        X11Screen(Screen&& ths) = delete;

        X11Screen& operator=(const Screen& rhs) = delete;
        X11Screen& operator=(Screen&& rhs) = delete;

        ~X11Screen() override {
            delete render_;
        };

        void init(RenderType renderType) override {
            std::print("X11Screen(): call init()\n");

            switch (renderType) {
            case RenderType::GL: {
                render_ = new RenderGL{};
                break;
            }
            case RenderType::VK: {
                render_ = new RenderVK{};
                break;
            }
            case RenderType::SF: {
                render_ = new RenderSF{};
                break;
            }
            default:
                break;
            };
        };

        [[nodiscard]] Render* getRender() override {
            std::print("X11Screen(): call getRender()\n");
            return render_;
        };

    private:
        Render* render_;
};
}  // namespace tire
