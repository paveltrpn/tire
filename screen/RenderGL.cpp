
module;

#include <string>
#include <iostream>
#include <format>
#include <print>

#include <GL/glew.h>

export module screen:RenderGL;

import :Render;

namespace tire {

struct __gl_Render : Render {
        void displayRenderInfo() override {
            std::print("vendor - {}\nrenderer - {}\nOpenGL version - {}\nGLSL version - {}\n",
                       vendor_,
                       renderer_,
                       glVersion_,
                       glslVersion_);
        }

    protected:
        std::string vendor_;
        std::string renderer_;
        std::string glVersion_;
        std::string glslVersion_;
};

}  // namespace tire
