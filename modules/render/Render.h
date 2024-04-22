
#ifndef __render_h__
#define __render_h__

#include <GLFW/glfw3.h>
#include "config/Config.h"

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace tire {

struct Render {
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
};

}  // namespace tire

#endif