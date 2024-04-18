
#ifndef __rendergl_h__
#define __rendergl_h__

#include <string>
#include <iostream>
#include <format>
#include <print>

#include "GLFW/glfw3.h"
#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "config/Config.h"
#include "Render.h"

namespace tire {

[[maybe_unused]]
static void GLAPIENTRY MessageCallback(GLenum source,
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
        __gl_Render(const tire::Config& config);

        void displayRenderInfo() override;

    protected:
        void setupDebugMessages();

        int ctxtVersionMajorMax_{};
        int ctxtVersionMinorMax_{};

        int ctxtVersionMajorUsed_{};
        int ctxtVersionMinorUsed_{};

        std::string vendor_;
        std::string renderer_;
        std::string glVersion_;
        std::string glslVersion_;
};

// ======================================================================================
// =============== OpenGL with GLFW initialization struct ===============================
// ======================================================================================
struct __glfw_gl_Render : __gl_Render {
        __glfw_gl_Render(GLFWwindow* window, const tire::Config& config);

        void preFrame() override;
        void postFrame() override;

    private:
        GLFWwindow* window_;
};

// ======================================================================================
// =============== OpenGL with X11 initialization struct ===============================
// ======================================================================================
struct __x11_gl_Render : __gl_Render {
        __x11_gl_Render(const tire::Config& config);

        void displayRenderInfo() override;
        void preFrame() override;
        void postFrame() override;

        ~__x11_gl_Render();

    private:
        Display* display_;
        Window window_;
        Colormap colorMap_;
};

}  // namespace tire

#endif