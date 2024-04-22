
#include <iostream>
#include <format>
#include <print>
#include <memory>

#include "render/Render.h"
#include "render/RenderGL.h"
#include "render/RenderVK.h"
#include "config/Config.h"

#include "spdlog/spdlog.h"

int main(int argc, char** argv) {
    auto configJson = R"foo(
{
      "render_type":"OpenGL",
      "application_name":"app",
      "fullscreen":false,
      "resizeable":true,
      "window_width":800,
      "window_height":600,
      "window_pos_x":100,
      "window_pos_y":100,
      "doublebuffer":true,
      "enable_vsync":true,
      "use_maximum_context_version":false,
      "use_context_version_major":3,
      "use_context_version_minor":2,
      "enable_debug":true,
      "application_name":"vk-app",
      "engine_name":"vk-eng",
      "enable_validation_layers":false
}
)foo";

    tire::Config config{ configJson };

    auto renderType = config.getString("render_type");

    std::unique_ptr<tire::Render> rndr;
    if (renderType == "OpenGL")
        rndr = std::make_unique<tire::RenderGL>(config);
    else if (renderType == "Vulkan")
        rndr = std::make_unique<tire::RenderVK>(config);
    else {
        spdlog::info("unknown render type");
        return 0;
    }

    rndr->displayRenderInfo();

    rndr->run();

    return 0;
}
