
#pragma once

#include <memory>
#include <list>
#include <print>

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "config/config.h"
#include "geometry/node.h"
#include "opengl_types/shader.h"
#include "opengl_types/shader_sources.h"
#include "camera/camera.h"
#include "node_list.h"

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
    using point_scalar_type = float;

    Render();

    Render( const Render &rhs ) = delete;
    Render( Render &&ths ) = delete;

    Render &operator=( const Render &rhs ) = delete;
    Render &operator=( Render &&rhs ) = delete;

    virtual ~Render();

    virtual void displayRenderInfo() = 0;
    void run();

    virtual void appendToRenderList( std::shared_ptr<tire::Node> node ) = 0;
    void addCamera( std::shared_ptr<tire::camera::Camera> cameraPtr ) {
        camera_ = cameraPtr;
    }

protected:
    virtual void initMainLoop() = 0;
    virtual void preFrame() = 0;
    virtual void frame() = 0;
    virtual void postFrame() = 0;
    virtual void swapBuffers() = 0;

    [[nodiscard]] bool isExtensionSupported( const char *extList,
                                             const char *extension );
    virtual void setSwapInterval( int interval ) = 0;

protected:
    bool run_{ true };

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

    // shader sources manager
    ShaderDatabase shaderSourcesManager_{};

    // render list
    NodeList renderList_;

    // cameras
    std::shared_ptr<camera::Camera> camera_{ nullptr };

private:
    void openDisplay();
    void configureX11();
};

}  // namespace tire
