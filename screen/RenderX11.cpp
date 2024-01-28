
module;

export module screen:RenderX11;

import :Render;
import :RenderGL;
import :RenderVK;
import :RenderSF;

namespace tire {

struct __x11_gl_Render : __gl_Render {
        __x11_gl_Render() = default;
        __x11_gl_Render(const __x11_gl_Render& rhs) = delete;
        __x11_gl_Render(__x11_gl_Render&& ths) = delete;

        __x11_gl_Render& operator=(const __x11_gl_Render& rhs) = delete;
        __x11_gl_Render& operator=(__x11_gl_Render&& rhs) = delete;

        ~__x11_gl_Render() override = default;
};

struct __x11_vk_Render : __vk_Render {
        __x11_vk_Render() = default;
        __x11_vk_Render(const __x11_vk_Render& rhs) = delete;
        __x11_vk_Render(__x11_vk_Render&& ths) = delete;

        __x11_vk_Render& operator=(const __x11_vk_Render& rhs) = delete;
        __x11_vk_Render& operator=(__x11_vk_Render&& rhs) = delete;

        ~__x11_vk_Render() override = default;
};

export struct __x11_sf_Render : __sf_Render {
        __x11_sf_Render() = default;
        __x11_sf_Render(const __x11_sf_Render& rhs) = delete;
        __x11_sf_Render(__x11_sf_Render&& ths) = delete;

        __x11_sf_Render& operator=(const __x11_sf_Render& rhs) = delete;
        __x11_sf_Render& operator=(__x11_sf_Render&& rhs) = delete;

        ~__x11_sf_Render() override = default;
};

}  // namespace tire
