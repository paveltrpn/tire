

#pragma once

#include <string>
#include <iostream>
#include <format>
#include <print>
#include <map>
#include <coroutine>

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "../render.h"
#include "scene.h"

import event;
import io;
import config;

namespace tire {

using namespace tire::gl;

struct RenderGL final : Render {
    RenderGL();
    ~RenderGL() override;

    void displayRenderInfo() override;

private:
    void checkGlxVersion();
    void initGlxExtensions();
    void configureGl();
    void setupDebugMessages();

    std::string getGLVendorString() {
        return { (const char *)glGetString( GL_VENDOR ) };
    }

    std::string getGLRendererString() {
        return { (const char *)glGetString( GL_RENDERER ) };
    }

    std::string getGLVersionString() {
        return { (const char *)glGetString( GL_VERSION ) };
    }

    std::string getGLSLVendorString() {
        return { (const char *)glGetString( GL_SHADING_LANGUAGE_VERSION ) };
    }

    void scene( const std::filesystem::path &path ) override;
    void preLoop() override;
    void preFrame() override;
    void frame() override;
    void postFrame() override;
    void swapBuffers() override;
    void postLoop() override;

    void setSwapInterval( int interval ) override;

    event::Task<void> showMetrics() {
        while ( true ) {
            co_await event::setTimeout( 3000 );
            event::Context::metrics();
            log::info( "average frame duration = {}",
                       timer_.averageFrameDuration() );
            timer_.resetFrameMetrics();
        }
    }

private:
    // glx extensions section
    using glXCreateContextAttribsARBProc = GLXContext ( * )( Display *,
                                                             GLXFBConfig,
                                                             GLXContext, Bool,
                                                             const int * );
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB{ nullptr };

    using glXSwapIntervalEXTProc = void ( * )( Display *, GLXDrawable, int );
    glXSwapIntervalEXTProc glXSwapIntervalEXT{ nullptr };
    static constexpr int GLX_SWAP_INTERVAL{ 0x20F1 };
    static constexpr int GLX_MAX_SWAP_INTEVAL{ 0x20F2 };

    // X11 OpenGL context
    GLXContext glContext_{ nullptr };

    std::shared_ptr<gl::Scene> scene_;
};

}  // namespace tire
