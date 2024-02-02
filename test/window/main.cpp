
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
      "enable_vsync":true,
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
    auto screenConfig = tire::Config{ config["Screen"] };

    auto screenType = screenConfig.getString("screen_type");

    std::unique_ptr<tire::Screen> scrn;
    if (screenType == "GLFW")
        scrn = std::make_unique<tire::GLFWScreen>(screenConfig);
    else if (screenType == "X11")
        scrn = std::make_unique<tire::X11Screen>(screenConfig);
    else
        throw std::runtime_error("unknown render type\n");

    scrn->setWindowPosX(500);

    auto renderType = screenConfig.getString("render_type");

    if (renderType == "OpenGL")
        scrn->initRender(tire::RenderType::OPENGL, config["OpenGL"]);
    else if (renderType == "Vulkan")
        scrn->initRender(tire::RenderType::VULKAN, config["Vulkan"]);
    else
        throw std::runtime_error("unknown render type\n");

    auto rndr = scrn->getRenderPtr();

    scrn->displayScreenInfo();
    rndr->displayRenderInfo();

    scrn->run();

    return 0;
}
