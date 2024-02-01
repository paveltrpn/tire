
module;

export module screen:RenderX11;

import :Render;
import :RenderGL;
import :RenderVK;

namespace tire {

struct __x11_gl_Render : __gl_Render {};

struct __x11_vk_Render : __vk_Render {};

}  // namespace tire
