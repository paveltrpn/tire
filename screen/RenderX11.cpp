
module;

export module screen:RenderX11;

import :Render;
import :RenderGL;
import :RenderVK;
import :RenderSF;

namespace tire {

struct __x11_gl_Render : __gl_Render {};

struct __x11_vk_Render : __vk_Render {};

export struct __x11_sf_Render : __sf_Render {};

}  // namespace tire
