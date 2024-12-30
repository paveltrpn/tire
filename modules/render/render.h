
#pragma once

#include <memory>
#include <list>
#include <print>
#include <filesystem>

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "uv.h"

#include "config/config.h"
#include "scene/scene.h"
#include "camera/camera.h"

namespace tire {

namespace __detail {
[[maybe_unused]] static void GLAPIENTRY MessageCallback(
    GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
    const GLchar *message, const void *userParam ) {
    std::print( "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type,
                severity, message );
}

static bool ctxErrorOccurred = false;
[[maybe_unused]] static int ctxErrorHandler( Display *dpy, XErrorEvent *ev ) {
    ctxErrorOccurred = true;
    return 0;
}

}  // namespace __detail

struct Render {
    Render();

    Render( const Render &rhs ) = delete;
    Render( Render &&ths ) = delete;

    Render &operator=( const Render &rhs ) = delete;
    Render &operator=( Render &&rhs ) = delete;

    virtual ~Render();

    virtual void displayRenderInfo() = 0;

    void scene( const std::filesystem::path &path );
    void run();

private:
    static void loop( uv_idle_t *handle );

protected:
    virtual void preLoop() = 0;
    virtual void preFrame() = 0;
    virtual void frame() = 0;
    virtual void postFrame() = 0;
    virtual void swapBuffers() = 0;
    virtual void postLoop() = 0;

    [[nodiscard]] bool isExtensionSupported( const char *extList,
                                             const char *extension );
    virtual void setSwapInterval( int interval ) = 0;

protected:
    bool run_{ true };
    uv_loop_t *loop_{ nullptr };
    uv_idle_t idler_;

    // X11
    Display *display_;
    Window window_;
    Colormap colorMap_;
    GLXFBConfig bestFbc_;

    // window properties
    int posx_{};
    int posy_{};
    int width_{};
    int height_{};

    std::shared_ptr<Scene> scene_{};

private:
    void openDisplay();
    void configureX11();
};

}  // namespace tire
