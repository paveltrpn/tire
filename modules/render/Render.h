
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

// Helper to check for extension string presence.  Adapted from:
//   http://www.opengl.org/resources/features/OGLextensions/
static bool isExtensionSupported(const char* extList, const char* extension) {
    const char* start;
    const char *where, *terminator;

    /* Extension names should not have spaces. */
    where = strchr(extension, ' ');
    if (where || *extension == '\0')
        return false;

    /* It takes a bit of care to be fool-proof about parsing the
       OpenGL extensions string. Don't be fooled by sub-strings,
       etc. */
    for (start = extList;;) {
        where = strstr(start, extension);

        if (!where)
            break;

        terminator = where + strlen(extension);

        if (where == start || *(where - 1) == ' ')
            if (*terminator == ' ' || *terminator == '\0')
                return true;

        start = terminator;
    }

    return false;
}

static bool ctxErrorOccurred = false;
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

        void appendToRenderList(std::shared_ptr<tire::node<point_scalar_type>> node);

    private:
        void configureX11();

    protected:
        bool run_{ true };
        tire::Config config_;

        // X11
        Display* display_;
        Window window_;
        Colormap colorMap_;
        GLXFBConfig bestFbc_;

        // render list
        std::list<std::shared_ptr<tire::node<point_scalar_type>>> renderList_;
};

}  // namespace tire

#endif
