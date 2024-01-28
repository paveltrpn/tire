
module;

export module screen:RenderSDL;

import :Render;
import :RenderGL;
import :RenderVK;
import :RenderSF;

namespace tire {

struct __sdl_gl_Render : __gl_Render {
        __sdl_gl_Render() = default;
        __sdl_gl_Render(const __sdl_gl_Render& rhs) = delete;
        __sdl_gl_Render(__sdl_gl_Render&& ths) = delete;

        __sdl_gl_Render& operator=(const __sdl_gl_Render& rhs) = delete;
        __sdl_gl_Render& operator=(__sdl_gl_Render&& rhs) = delete;

        ~__sdl_gl_Render() override = default;
};

struct __sdl_vk_Render : __vk_Render {
        __sdl_vk_Render() = default;
        __sdl_vk_Render(const __sdl_vk_Render& rhs) = delete;
        __sdl_vk_Render(__sdl_vk_Render&& ths) = delete;

        __sdl_vk_Render& operator=(const __sdl_vk_Render& rhs) = delete;
        __sdl_vk_Render& operator=(__sdl_vk_Render&& rhs) = delete;

        ~__sdl_vk_Render() override = default;
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
