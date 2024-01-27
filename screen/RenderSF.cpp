
module;

#include <iostream>
#include <format>
#include <print>

export module screen:RenderSF;

import :Render;

namespace tire {
export struct __sf_Render : Render {
        __sf_Render() = default;
        __sf_Render(const __sf_Render& rhs) = delete;
        __sf_Render(__sf_Render&& ths) = delete;

        __sf_Render& operator=(const __sf_Render& rhs) = delete;
        __sf_Render& operator=(__sf_Render&& rhs) = delete;

        ~__sf_Render() override = default;
};

export struct __glfw_sf_Render : __sf_Render {
        __glfw_sf_Render() = default;
        __glfw_sf_Render(const __glfw_sf_Render& rhs) = delete;
        __glfw_sf_Render(__glfw_sf_Render&& ths) = delete;

        __glfw_sf_Render& operator=(const __glfw_sf_Render& rhs) = delete;
        __glfw_sf_Render& operator=(__glfw_sf_Render&& rhs) = delete;

        ~__glfw_sf_Render() override = default;
};

export struct __sdl_sf_Render : __sf_Render {
        __sdl_sf_Render() = default;
        __sdl_sf_Render(const __sdl_sf_Render& rhs) = delete;
        __sdl_sf_Render(__sdl_sf_Render&& ths) = delete;

        __sdl_sf_Render& operator=(const __sdl_sf_Render& rhs) = delete;
        __sdl_sf_Render& operator=(__sdl_sf_Render&& rhs) = delete;

        ~__sdl_sf_Render() override = default;
};
}  // namespace tire
