
#ifndef __rendergl_h__
#define __rendergl_h__

#include <string>
#include <iostream>
#include <format>
#include <print>

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "config/Config.h"
#include "Render.h"
#include "GLFunctions.h"

namespace tire {

struct RenderGL : Render {
        RenderGL(const tire::Config& config);
        ~RenderGL() override;

        void displayRenderInfo() override;
        void preFrame() override;
        void postFrame() override;

        void swapBuffers() override;

    private:
        void configureGl();

        void setupDebugMessages();

        GLXContext glContext_{ nullptr };

        // OpeneGL function pointers handler object
        std::shared_ptr<GLFunctions> gl;

        std::string vendor_;
        std::string renderer_;
        std::string glVersion_;
        std::string glslVersion_;
};

}  // namespace tire

#endif
