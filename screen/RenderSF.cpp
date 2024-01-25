
module;

#include <iostream>
#include <format>
#include <print>

export module screen:RenderSF;

import :Render;

namespace tire {
export struct RenderSF : Render {
        RenderSF() = default;
        RenderSF(const RenderSF& rhs) = delete;
        RenderSF(RenderSF&& ths) = delete;

        RenderSF& operator=(const RenderSF& rhs) = delete;
        RenderSF& operator=(RenderSF&& rhs) = delete;

        ~RenderSF() override = default;

        void run() override {
            std::print("RenderSF(): call run()\n");
        };
};
}  // namespace tire
