
#include <iostream>
#include <format>
#include <print>
#include <memory>

#include "nlohmann/json.hpp"

import screen;
import render;
import config;

int main(int argc, char** argv) {
    auto configJson = R"(
{
   "Screen":{
      "screen_type":"GLFW",
      "render_type":"Vulkan",
      "application_name":"app",
      "fullscreen":false,
      "resizeable":false,
      "window_width":800,
      "window_height":600,
      "window_pos_x":100,
      "window_pos_y":100
   },
   "OpnenGL":{
      "use_maximum_context_version":true,
      "use_context_version_major":2,
      "use_context_version_minor":1,
      "enable_debug":true
   },
   "Vulkan":{
      "application_name":"vk-app",
      "engine_name":"vk-eng",
      "enable_validation_layers":true
   }
}
)";

    nlohmann::json config = nlohmann::json::parse(configJson);

    auto screenType = std::string{ config["Screen"]["screen_type"] };

    std::unique_ptr<tire::Screen> sc;
    if (screenType == "GLFW")
        sc = std::make_unique<tire::GLFWScreen>(config["Screen"]);
    else if (screenType == "X11")
        sc = std::make_unique<tire::X11Screen>(config["Screen"]);
    else
        throw std::runtime_error("unknown render type\n");

    sc->setWindowPosX(500);

    auto renderType = std::string{ config["Screen"]["render_type"] };
    if (renderType == "OpenGL")
        sc->initRender(tire::RenderType::OPENGL, config["OpenGL"]);
    else if (renderType == "Vulkan")
        sc->initRender(tire::RenderType::VULKAN, config["Vulkan"]);
    else
        throw std::runtime_error("unknown render type\n");

    sc->displayScreenInfo();
    sc->displayRenderInfo();

    sc->run();

    return 0;
}
