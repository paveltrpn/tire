
#ifndef __render_h__
#define __render_h__

#include <memory>
#include <list>

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "config/Config.h"
#include "geometry/Node.h"
#include "geometry/Point.h"
#include "GLFunctions.h"

namespace tire {

namespace __detail_tire {
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

static bool ctxErrorOccurred = false;
[[maybe_unused]]
static int ctxErrorHandler(Display* dpy, XErrorEvent* ev) {
    ctxErrorOccurred = true;
    return 0;
}

}  // namespace __detail_tire

struct Render {
        using point_scalar_type = float;

        Render() = delete;

        Render(const tire::Config& config);

        Render(const Render& rhs) = delete;
        Render(Render&& ths) = delete;

        Render& operator=(const Render& rhs) = delete;
        Render& operator=(Render&& rhs) = delete;

        virtual ~Render();

        virtual void displayRenderInfo() = 0;

        virtual void preFrame() = 0;
        virtual void postFrame() = 0;
        virtual void swapBuffers() = 0;

        void run();
        void frame();

        virtual void appendToRenderList(std::shared_ptr<tire::Node<point_scalar_type>> node) = 0;

    private:
        void openDisplay();
        void checkGlxVersion();
        void initGlxExtensions();
        void configureX11();

    protected:
        [[nodiscard]]
        bool isExtensionSupported(const char* extList, const char* extension);
        void setSwapInterval(int interval);

        bool run_{ true };
        tire::Config config_;

        // X11
        Display* display_;
        Window window_;
        Colormap colorMap_;
        GLXFBConfig bestFbc_;

        // render list
        std::list<std::shared_ptr<tire::Node<point_scalar_type>>> renderList_;

        // glx extensions section
    protected:
        using glXCreateContextAttribsARBProc
          = GLXContext (*)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
        glXCreateContextAttribsARBProc glXCreateContextAttribsARB{ nullptr };

        using glXSwapIntervalEXTProc = void (*)(Display*, GLXDrawable, int);
        glXSwapIntervalEXTProc glXSwapIntervalEXT{ nullptr };
        static constexpr int GLX_SWAP_INTERVAL{ 0x20F1 };
        static constexpr int GLX_MAX_SWAP_INTEVAL{ 0x20F2 };
};

}  // namespace tire

#endif
