
module;

export module screen:RenderSDL;

import :Render;
import :RenderGL;
import :RenderVK;
import :RenderSF;

namespace tire {

struct __sdl_gl_Render : __gl_Render {};

struct __sdl_vk_Render : __vk_Render {};

export struct __sdl_sf_Render : __sf_Render {};

}  // namespace tire
