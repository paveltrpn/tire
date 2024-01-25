
module;

#include <iostream>
#include <format>
#include <print>

export module screen:RenderVK;

import :Render;

namespace tire {
export struct RenderVK : Render {
        RenderVK() = default;
        RenderVK(const RenderVK& rhs) = delete;
        RenderVK(RenderVK&& ths) = delete;

        RenderVK& operator=(const RenderVK& rhs) = delete;
        RenderVK& operator=(RenderVK&& rhs) = delete;

        ~RenderVK() override = default;

        void run() override {
            std::print("RenderVK(): call run()\n");
        };
};
}  // namespace tire
