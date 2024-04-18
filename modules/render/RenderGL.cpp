
#include <string>
#include <iostream>
#include <format>
#include <print>

#include "GLFW/glfw3.h"

#include "config/Config.h"
#include "Render.h"
#include "RenderGL.h"
#include "spdlog/spdlog.h"
#include "spdlog/common.h"

namespace tire {

__gl_Render::__gl_Render(const tire::Config& config) : Render{ config } {
    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(&MessageCallback, nullptr);
}

void __gl_Render::displayRenderInfo() {
    spdlog::info("OpenGL context info\nvendor - {}\nrenderer - {}\nOpenGL version - {}\nGLSL version - {}",
                 vendor_,
                 renderer_,
                 glVersion_,
                 glslVersion_);
}

void __gl_Render::preFrame() {
    glClearColor(0, 0.5, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void __gl_Render::postFrame() {
}

void __gl_Render::setupDebugMessages() {
}

// ======================================================================================
// =============== OpenGL with GLFW initialization struct ===============================
// ======================================================================================
__glfw_gl_Render::__glfw_gl_Render(GLFWwindow* window, const tire::Config& config)
    : __gl_Render{ config }, window_{ window } {
    if (!window_) {
        throw std::runtime_error("GLFW window pointer can't be null!\n");
    }

    glGetIntegerv(GLFW_CONTEXT_VERSION_MAJOR, &ctxtVersionMajorMax_);
    glGetIntegerv(GLFW_CONTEXT_VERSION_MINOR, &ctxtVersionMinorMax_);

    if (config_.get<bool>("use_maximum_context_version")) {
        ctxtVersionMajorUsed_ = ctxtVersionMajorMax_;
        ctxtVersionMinorUsed_ = ctxtVersionMinorMax_;
    } else {
        ctxtVersionMajorUsed_ = config_.get<int>("use_context_version_major");
        ctxtVersionMinorUsed_ = config_.get<int>("use_context_version_minor");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ctxtVersionMajorUsed_);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ctxtVersionMinorUsed_);

    if (doublebuffer_) {
        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    } else {
        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
    }

    glfwMakeContextCurrent(window_);

    // vsync off
    if (config_.get<bool>("enable_vsync")) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    vendor_ = (const char*)glGetString(GL_VENDOR);
    renderer_ = (const char*)glGetString(GL_RENDERER);
    glVersion_ = (const char*)glGetString(GL_VERSION);
    glslVersion_ = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
};

void __glfw_gl_Render::swapBuffers() {
    glfwSwapBuffers(window_);
};

// ======================================================================================
// =============== OpenGL with X11 initialization struct ================================
// ======================================================================================

#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092

typedef GLXContext (
  *glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

__x11_gl_Render::__x11_gl_Render(const tire::Config& config) : __gl_Render(config) {
    display_ = XOpenDisplay(nullptr);

    if (!display_) {
        std::runtime_error("failed to open X display");
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

    int fbcount;
    GLXFBConfig* fbc
      = glXChooseFBConfig(display_, DefaultScreen(display_), visual_attribs, &fbcount);
    if (!fbc) {
        throw std::runtime_error("failed to retrieve a framebuffer config\n");
    }
    spdlog::info("Found {} matching FB configs", fbcount);

    // Pick the FB config/visual with the most samples per pixel
    spdlog::info("getting XVisualInfos");
    int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

    int i;
    for (i = 0; i < fbcount; ++i) {
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

    GLXFBConfig bestFbc = fbc[best_fbc];

    // Be sure to free the FBConfig list allocated by glXChooseFBConfig()
    XFree(fbc);

    // Get a visual
    XVisualInfo* vi = glXGetVisualFromFBConfig(display_, bestFbc);
    spdlog::info("chosen visual ID = {}", vi->visualid);

    spdlog::info("creating colormap");
    XSetWindowAttributes swa;
    swa.colormap = colorMap_
      = XCreateColormap(display_, RootWindow(display_, vi->screen), vi->visual, AllocNone);
    swa.background_pixmap = None;
    swa.border_pixel = 0;
    swa.event_mask = StructureNotifyMask;

    spdlog::info("creating window");
    window_ = XCreateWindow(display_,
                            RootWindow(display_, vi->screen),
                            0,
                            0,
                            100,
                            100,
                            0,
                            vi->depth,
                            InputOutput,
                            vi->visual,
                            CWBorderPixel | CWColormap | CWEventMask,
                            &swa);
    if (!window_) {
        std::runtime_error("failed to create window");
    }

    // Done with the visual info data
    XFree(vi);

    XStoreName(display_, window_, "GL 3.0 Window");

    spdlog::info("mapping window");
    XMapWindow(display_, window_);

    // Get the default screen's GLX extension list
    const char* glxExts = glXQueryExtensionsString(display_, DefaultScreen(display_));

    // NOTE: It is not necessary to create or make current to a context before
    // calling glXGetProcAddressARB
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB(
      (const GLubyte*)"glXCreateContextAttribsARB");

    GLXContext ctx = 0;

    // Install an X error handler so the application won't exit if GL 3.0
    // context allocation fails.
    //
    // Note this error handler is global.  All display connections in all threads
    // of a process use the same error handler, so be sure to guard against other
    // threads issuing X commands while this code is running.
    __detail::ctxErrorOccurred = false;
    int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&__detail::ctxErrorHandler);

    // Check for the GLX_ARB_create_context extension string and the function.
    // If either is not present, use GLX 1.3 context creation method.
    if (!__detail::isExtensionSupported(glxExts, "GLX_ARB_create_context")
        || !glXCreateContextAttribsARB) {
        printf("glXCreateContextAttribsARB() not found"
               " ... using old-style GLX context\n");
        ctx = glXCreateNewContext(display_, bestFbc, GLX_RGBA_TYPE, 0, True);
    }

    // If it does, try to get a GL 3.0 context!
    else {
        int context_attribs[]
          = { GLX_CONTEXT_MAJOR_VERSION_ARB,
              3,
              GLX_CONTEXT_MINOR_VERSION_ARB,
              0,
              // GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
              None };

        spdlog::info("creating context");
        ctx = glXCreateContextAttribsARB(display_, bestFbc, 0, True, context_attribs);

        // Sync to ensure any errors generated are processed.
        XSync(display_, False);
        if (!__detail::ctxErrorOccurred && ctx)
            printf("Created GL 3.0 context\n");
        else {
            // Couldn't create GL 3.0 context.  Fall back to old-style 2.x context.
            // When a context version below 3.0 is requested, implementations will
            // return the newest context version compatible with OpenGL versions less
            // than version 3.0.
            // GLX_CONTEXT_MAJOR_VERSION_ARB = 1
            context_attribs[1] = 1;
            // GLX_CONTEXT_MINOR_VERSION_ARB = 0
            context_attribs[3] = 0;

            __detail::ctxErrorOccurred = false;

            spdlog::info("Failed to create GL 3.0 context... using old-style GLX context");
            ctx = glXCreateContextAttribsARB(display_, bestFbc, 0, True, context_attribs);
        }
    }

    // Sync to ensure any errors generated are processed.
    XSync(display_, False);

    // Restore the original error handler
    XSetErrorHandler(oldHandler);

    if (__detail::ctxErrorOccurred || !ctx) {
        std::runtime_error("failed to create an OpenGL context");
    }

    // Verifying that context is a direct context
    if (!glXIsDirect(display_, ctx)) {
        spdlog::info("Indirect GLX rendering context obtained");
    } else {
        spdlog::info("Direct GLX rendering context obtained");
    }

    spdlog::info("Making context current");
    glXMakeCurrent(display_, window_, ctx);

    vendor_ = (const char*)glGetString(GL_VENDOR);
    renderer_ = (const char*)glGetString(GL_RENDERER);
    glVersion_ = (const char*)glGetString(GL_VERSION);
    glslVersion_ = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
};

__x11_gl_Render::~__x11_gl_Render() {
    XDestroyWindow(display_, window_);
    XFreeColormap(display_, colorMap_);
    XCloseDisplay(display_);
}

void __x11_gl_Render::swapBuffers() {
    glXSwapBuffers(display_, window_);
};

}  // namespace tire
