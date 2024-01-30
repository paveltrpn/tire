
module;

#include <iostream>
#include <format>
#include <print>

export module screen:RenderSF;

import :Render;

namespace tire {
export struct __sf_Render : Render {
        void displayRenderInfo() override {
            std::print("SF render not implemented\n");
        };

        void swapBuffers() override{};
};
}  // namespace tire
