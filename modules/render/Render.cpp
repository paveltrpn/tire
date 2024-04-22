
#include <GLFW/glfw3.h>

#include "config/Config.h"
#include "Render.h"

#include "spdlog/spdlog.h"

namespace tire {

Render::Render(const tire::Config& config) : config_{ config } {
    configureX11();
}

Render::~Render() {
    XDestroyWindow(display_, window_);
    XFreeColormap(display_, colorMap_);
    XCloseDisplay(display_);
}

void Render::configureX11() {
    display_ = XOpenDisplay(nullptr);

    if (!display_) {
        throw std::runtime_error("failed to open X display");
    }

    // Get a matching FB config
    constexpr int visual_attribs[] = { GLX_X_RENDERABLE,
                                       True,
                                       GLX_DRAWABLE_TYPE,
                                       GLX_WINDOW_BIT,
                                       GLX_RENDER_TYPE,
                                       GLX_RGBA_BIT,
                                       GLX_X_VISUAL_TYPE,
                                       GLX_TRUE_COLOR,
                                       GLX_RED_SIZE,
                                       8,
                                       GLX_GREEN_SIZE,
                                       8,
                                       GLX_BLUE_SIZE,
                                       8,
                                       GLX_ALPHA_SIZE,
                                       8,
                                       GLX_DEPTH_SIZE,
                                       24,
                                       GLX_STENCIL_SIZE,
                                       8,
                                       GLX_DOUBLEBUFFER,
                                       True,
                                       // GLX_SAMPLE_BUFFERS  , 1,
                                       // GLX_SAMPLES         , 4,
                                       None };

    int glx_major, glx_minor;

    // FBConfigs were added in GLX version 1.3.
    if (!glXQueryVersion(display_, &glx_major, &glx_minor) || ((glx_major == 1) && (glx_minor < 3))
        || (glx_major < 1)) {
        throw std::runtime_error("invalid GLX version");
    }

    spdlog::info("glx version: {}.{}", glx_major, glx_minor);

    int fbcount;
    GLXFBConfig* fbc
      = glXChooseFBConfig(display_, DefaultScreen(display_), visual_attribs, &fbcount);
    if (!fbc) {
        throw std::runtime_error("failed to retrieve a framebuffer config\n");
    }

    // Pick the FB config/visual with the most samples per pixel
    spdlog::info("getting XVisualInfos");
    int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

    for (auto i = 0; i < fbcount; ++i) {
        XVisualInfo* vi = glXGetVisualFromFBConfig(display_, fbc[i]);
        if (vi) {
            int samp_buf, samples;
            glXGetFBConfigAttrib(display_, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
            glXGetFBConfigAttrib(display_, fbc[i], GLX_SAMPLES, &samples);

            spdlog::info("matching fbconfig {}, visual ID {}: SAMPLE_BUFFERS = {},"
                         " SAMPLES = {}",
                         i,
                         vi->visualid,  // TODO: need to be HEX
                         samp_buf,
                         samples);

            if (best_fbc < 0 || (samp_buf && samples > best_num_samp))
                best_fbc = i, best_num_samp = samples;
            if (worst_fbc < 0 || !samp_buf || samples < worst_num_samp)
                worst_fbc = i, worst_num_samp = samples;
        }
        XFree(vi);
    }

    // choose best framebuffer config
    bestFbc_ = fbc[best_fbc];

    // Be sure to free the FBConfig list allocated by glXChooseFBConfig()
    XFree(fbc);

    // Get a visual
    XVisualInfo* vi = glXGetVisualFromFBConfig(display_, bestFbc_);
    spdlog::info("chosen visual ID = {}", vi->visualid);

    // create colormap
    XSetWindowAttributes swa;
    swa.colormap = colorMap_
      = XCreateColormap(display_, RootWindow(display_, vi->screen), vi->visual, AllocNone);
    swa.background_pixmap = None;
    swa.border_pixel = 0;
    swa.event_mask = StructureNotifyMask;

    // create window
    window_ = XCreateWindow(display_,
                            RootWindow(display_, vi->screen),
                            config_.get<int>("window_pos_x", 100),
                            config_.get<int>("window_pos_y", 100),
                            config_.get<int>("window_width", 320),
                            config_.get<int>("window_height", 240),
                            0,
                            vi->depth,
                            InputOutput,
                            vi->visual,
                            CWBorderPixel | CWColormap | CWEventMask,
                            &swa);

    if (!window_) {
        throw std::runtime_error("failed to create window");
    }

    XFree(vi);

    XStoreName(display_, window_, config_.get<std::string>("application_name", "default").c_str());
    XMapWindow(display_, window_);
}

void Render::frame() {
    preFrame();

    postFrame();
    swapBuffers();
}

void Render::run() {
    XSelectInput(display_, window_, KeyPressMask | KeyReleaseMask);
    while (run_) {
        while (XPending(display_)) {
            XEvent KeyEvent;
            XNextEvent(display_, &KeyEvent);
            if (KeyEvent.type == KeyPress) {
                auto keyEventCode = KeyEvent.xkey.keycode;
                switch (keyEventCode) {
                case 9: {  // == ESCAPE
                    run_ = false;
                    break;
                }
                default:
                    break;
                }

            } else if (KeyEvent.type == KeyRelease) {
                auto keyEventCode = KeyEvent.xkey.keycode;
                switch (keyEventCode) {
                default:
                    break;
                }
            }
        }
        frame();
    }
}

void Render::appendToRenderList(std::shared_ptr<tire::node<tire::point3f>> node) {
    renderList_.push_back(node);
}

}  // namespace tire
