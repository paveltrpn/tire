
module;

export module render:RenderX11;

import :Render;
import :RenderGL;
import :RenderVK;

namespace tire {

struct __x11_gl_Render : __gl_Render {
        void displayRenderInfo() override {};
        void swapBuffers() override {};
};

struct __x11_vk_Render : __vk_Render {
        void displayRenderInfo() override {};
        void swapBuffers() override {};
};

}  // namespace tire
