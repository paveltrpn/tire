
module;

#include <string>
#include <iostream>
#include <format>
#include <print>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

export module render:RenderGL;

import config;
import :Render;

namespace tire {
void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam) {
    std::print("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
               (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
               type,
               severity,
               message);
}

struct __gl_Render : Render {
        __gl_Render(const tire::Config& config): Render{config} {
            // glEnable(GL_DEBUG_OUTPUT);
            // glDebugMessageCallback(&MessageCallback, nullptr);
        }

        void displayRenderInfo() override {
            std::print("vendor - {}\nrenderer - {}\nOpenGL version - {}\nGLSL version - {}\n",
                       vendor_,
                       renderer_,
                       glVersion_,
                       glslVersion_);
        }

    protected:
        void setupDebugMessages() {
        }

        int ctxtVersionMajorMax_{};
        int ctxtVersionMinorMax_{};

        int ctxtVersionMajorUsed_{};
        int ctxtVersionMinorUsed_{};

        std::string vendor_;
        std::string renderer_;
        std::string glVersion_;
        std::string glslVersion_;
};

}  // namespace tire
