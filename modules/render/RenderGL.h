
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

#include "shader/Shader.h"

namespace tire {

struct RenderGL final : Render {
        RenderGL();
        ~RenderGL() override;

        void displayRenderInfo() override;

        void appendToRenderList(std::shared_ptr<tire::Node<point_scalar_type>> node) override;

    private:
        void configureGl();
        void setupDebugMessages();

        void initMainLoop() override;
        void preFrame() override;
        void frame() override;
        void postFrame() override;
        void swapBuffers() override;

        void prepareShaders();

    private:
        GLXContext glContext_{ nullptr };

        std::string vendor_{};
        std::string renderer_{};
        std::string glVersion_{};
        std::string glslVersion_{};

        GLuint bufferObject_{};
        GLuint vertexObject_{};

        std::map<ShaderID, Shader> programs_;
};

}  // namespace tire

#endif
