
#include <iostream>
#include <format>
#include <print>
#include <memory>

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

import screen;
import render;

int main(int argc, char** argv) {
    auto configJson = R"(
{
   "Screen":{
      "type":"GLFW",
      "application_name":"app",
      "fullscreen":false,
      "resizeable":false,
      "window_width":800,
      "window_height":600,
      "window_pos_x":100,
      "window_pos_y":100
   },
   "Render":{
      "type":"Vulkan",
      "resizeable":false,
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

    std::unique_ptr<tire::Screen> sc;
    auto screenType = std::string{ config["Screen"]["type"] };
    if (screenType == "GLFW")
        sc = std::make_unique<tire::GLFWScreen>(config);
    else if (screenType == "X11")
        sc = std::make_unique<tire::X11Screen>(config);
    else
        throw std::runtime_error("unknown render type\n");

    sc->setWindowPosX(500);

    auto renderType = std::string{ config["Render"]["type"] };
    if (renderType == "OpenGL")
        sc->init(tire::RenderType::OPENGL);
    else if (renderType == "Vulkan")
        sc->init(tire::RenderType::VULKAN);
    else
        throw std::runtime_error("unknown render type\n");

    sc->displayScreenInfo();
    sc->displayRenderInfo();

    sc->run();

    return 0;
}
