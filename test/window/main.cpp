
#include <iostream>
#include <format>
#include <print>
#include <memory>

#include "spdlog/spdlog.h"

#include "config/Config.h"
#include "render/Render.h"
#include "screen/X11Screen.h"
#include "screen/GLFWScreen.h"

int main(int argc, char** argv) {
    auto configJson = R"foo(
{
   "Screen":{
      "screen_type":"X11",
      "render_type":"OpenGL",
      "application_name":"app",
      "fullscreen":false,
      "resizeable":true,
      "window_width":800,
      "window_height":600,
      "window_pos_x":100,
      "window_pos_y":100
   },
   "Render":{
      "doublebuffer":true,
      "enable_vsync":true,
      "use_maximum_context_version":true,
      "use_context_version_major":2,
      "use_context_version_minor":1,
      "enable_debug":true,
      "application_name":"vk-app",
      "engine_name":"vk-eng",
      "enable_validation_layers":false
   },
   "Input":{}
}
)foo";

    tire::Config config{ configJson };

    auto screenConfig = tire::Config{ config.get<tire::json>("Screen") };
    auto renderConfig = tire::Config{ config.get<tire::json>("Render") };

    auto screenType = screenConfig.get<std::string>("screen_type");

    std::unique_ptr<tire::Screen> scrn;
    if (screenType == "GLFW")
        scrn = std::make_unique<tire::GLFWScreen>(screenConfig);
    else if (screenType == "X11")
        scrn = std::make_unique<tire::X11Screen>(screenConfig);
    else {
        spdlog::info("unknown screen type");
        return 0;
    }

    scrn->setWindowPosX(500);

    auto renderType = screenConfig.getString("render_type");

    if (renderType == "OpenGL")
        scrn->initRender(tire::RenderType::OPENGL, renderConfig);
    else if (renderType == "Vulkan")
        scrn->initRender(tire::RenderType::VULKAN, renderConfig);
    else {
        spdlog::info("unknown render type");
        return 0;
    }

    auto rndr = scrn->getRenderPtr();

    scrn->displayScreenInfo();
    rndr->displayRenderInfo();

    scrn->run();

    return 0;
}
