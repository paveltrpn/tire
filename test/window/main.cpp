
#include <iostream>
#include <format>
#include <print>
#include <memory>

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

import screen;

int main(int argc, char** argv) {
    auto configJson = R"(
{
   "Screen":{
      "type":"GLFW",
      "application_name":"app",
      "window_width":800,
      "window_height":600,
      "window_pos_x":100,
      "window_pos_y":100
   },
   "Render":{
      "type":"Vulkan",
      "OpnenGL":{
         "use_maximum_context_version":true,
         "use_context_version_major":2,
         "use_context_version_minor":1,
         "enable_debug":true
      },
      "Vulkan":{
         "enable_validation_layers":true
      }
   }
}
)";

    nlohmann::json config = nlohmann::json::parse(configJson);

    auto sc = std::make_unique<tire::GLFWScreen>("first");

    sc->setWindowPosX(500);

    auto render = std::string{ config["Render"]["type"] };

    if (render == "OpenGL")
        sc->init(tire::RenderType::OPENGL);
    else if (render == "Vulkan")
        sc->init(tire::RenderType::VULKAN);
    else
        throw std::runtime_error("unknown render type\n");

    sc->displayScreenInfo();
    sc->displayRenderInfo();

    sc->run();

    return 0;
}
