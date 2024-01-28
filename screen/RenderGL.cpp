
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
        __gl_Render() = default;
        __gl_Render(const __gl_Render& rhs) = delete;
        __gl_Render(__gl_Render&& ths) = delete;

        __gl_Render& operator=(const __gl_Render& rhs) = delete;
        __gl_Render& operator=(__gl_Render&& rhs) = delete;

        ~__gl_Render() override = default;

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
