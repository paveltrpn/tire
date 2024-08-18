
#ifndef __rendergl_h__
#define __rendergl_h__

#include <string>
#include <iostream>
#include <format>
#include <print>
#include <map>

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "Render.h"

import opengl_types;
import config;

namespace tire {

using namespace tire::opengl;

struct RenderGL final : Render {
        RenderGL();
        ~RenderGL() override;

        void displayRenderInfo() override;

        void appendToRenderList(std::shared_ptr<tire::Node<point_scalar_type>> node) override;

    private:
        void checkGlxVersion();
        void initGlxExtensions();
        void configureGl();
        void setupDebugMessages();

        std::string getGLVendorString() {
            return { (const char *)glGetString(GL_VENDOR) };
        }

        std::string getGLRendererString() {
            return { (const char *)glGetString(GL_RENDERER) };
        }

        std::string getGLVersionString() {
            return { (const char *)glGetString(GL_VERSION) };
        }

        std::string getGLSLVendorString() {
            return { (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION) };
        }

        void initMainLoop() override;
        void preFrame() override;
        void frame() override;
        void postFrame() override;
        void swapBuffers() override;

        void setSwapInterval(int interval) override;

        void prepareShaders();

    private:
        // glx extensions section
        using glXCreateContextAttribsARBProc
          = GLXContext (*)(Display *, GLXFBConfig, GLXContext, Bool, const int *);
        glXCreateContextAttribsARBProc glXCreateContextAttribsARB{ nullptr };

        using glXSwapIntervalEXTProc = void (*)(Display *, GLXDrawable, int);
        glXSwapIntervalEXTProc glXSwapIntervalEXT{ nullptr };
        static constexpr int GLX_SWAP_INTERVAL{ 0x20F1 };
        static constexpr int GLX_MAX_SWAP_INTEVAL{ 0x20F2 };

        // X11 OpenGL context
        GLXContext glContext_{ nullptr };

        GLuint bufferObject_{};
        GLuint vertexObject_{};

        std::map<ShaderID, Shader> programs_;
};

}  // namespace tire

#endif
