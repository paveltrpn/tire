
#include <iostream>
#include <format>
#include <print>
#include <memory>

#include "render/Render.h"
#include "render/RenderGL.h"
#include "render/RenderVK.h"
#include "config/Config.h"
#include "geometry/Point.h"
#include "geometry/Node.h"

#include "spdlog/spdlog.h"

constexpr auto configJson = R"foo(
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
      "engine_name":"vk-eng",
      "enable_validation_layers":false
}
)foo";

int main(int argc, char** argv) {
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

    auto box_one = std::make_shared<tire::node<float>>();
    box_one->setVerteciesArray({ tire::point3f{ 1.0f, 1.0f, 1.0f },
                                 tire::point3f{ 1.0f, -1.0f, 1.0f },
                                 tire::point3f{ -1.0f, -1.0f, 1.0f },
                                 tire::point3f{ -1.0f, 1.0f, 1.0f },
                                 tire::point3f{ 1.0f, 1.0f, -1.0f },
                                 tire::point3f{ 1.0f, -1.0f, -1.0f },
                                 tire::point3f{ -1.0f, -1.0f, -1.0f },
                                 tire::point3f{ -1.0f, 1.0f, -1.0f } });

    box_one->setIndicesArray({ 0, 1, 2, 3, 0, 2, 4, 5, 6, 7, 4, 6 });

    rndr->appendToRenderList(box_one);

    rndr->run();

    return 0;
}
