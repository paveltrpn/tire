
module;

#include <iostream>
#include <format>
#include <print>

export module screen:RenderGL;

import :Render;

namespace tire {
export struct RenderGL : Render {
        RenderGL() = default;
        RenderGL(const RenderGL& rhs) = delete;
        RenderGL(RenderGL&& ths) = delete;

        RenderGL& operator=(const RenderGL& rhs) = delete;
        RenderGL& operator=(RenderGL&& rhs) = delete;

        ~RenderGL() override = default;

        void run() override {
            std::print("RenderGL(): call run()\n");
        };
};
}  // namespace tire
