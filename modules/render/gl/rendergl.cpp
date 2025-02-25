

#include <GL/glx.h>
#include <memory>
#include <string>
#include <iostream>
#include <format>
#include <print>
#include <map>

#include "log/log.h"
#include "render/gl/screen_string.h"
#include "rendergl.h"

static constexpr bool DEBUG_OUTPUT_RENDERGL_CPP{ true };

namespace tire {

namespace __details {
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

}  // namespace __details

using namespace tire::gl;

RenderGL::RenderGL()
    : Render{} {
    checkGlxVersion();
    initGlxExtensions();
    configureGl();
    gl::init();
    setSwapInterval( 1 );

    screenString_ =
        std::make_unique<ScreenString>( "RobotoMono-1024-512-32-64.tga" );
}

RenderGL::~RenderGL() {
    glXDestroyContext( display_, glContext_ );
}

void RenderGL::checkGlxVersion() {
    int glx_major, glx_minor;

    // FBConfigs were added in GLX version 1.3.
    if ( !glXQueryVersion( display_, &glx_major, &glx_minor ) ||
         ( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) ) {
        throw std::runtime_error( "invalid GLX version" );
    }

    log::info( "glx version: {}.{}", glx_major, glx_minor );
}

void RenderGL::initGlxExtensions() {
    // Get the default screen's GLX extension list
    const char *glxExts =
        glXQueryExtensionsString( display_, DefaultScreen( display_ ) );

    glXCreateContextAttribsARB =
        (glXCreateContextAttribsARBProc)glXGetProcAddressARB(
            (const GLubyte *)"glXCreateContextAttribsARB" );

    if ( !isExtensionSupported( glxExts, "GLX_ARB_create_context" ) ||
         !glXCreateContextAttribsARB ) {
        throw std::runtime_error(
            "extension glXCreateContextAttribsARB not supported!" );
    }

    glXSwapIntervalEXT = (glXSwapIntervalEXTProc)glXGetProcAddressARB(
        (const GLubyte *)"glXSwapIntervalEXT" );

    if ( !isExtensionSupported( glxExts, "GLX_EXT_swap_control" ) ||
         !glXSwapIntervalEXT ) {
        throw std::runtime_error(
            "extension glXSwapIntervalEXT not supported!" );
    }
}

void RenderGL::setSwapInterval( int interval ) {
    GLXDrawable drawable = glXGetCurrentDrawable();
    if ( drawable ) {
        glXSwapIntervalEXT( display_, drawable, interval );
    }
}

void RenderGL::configureGl() {
    auto configHandle = Config::instance();

    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(&MessageCallback, nullptr);

    __details::ctxErrorOccurred = false;
    int ( *oldHandler )( Display *, XErrorEvent * ) =
        XSetErrorHandler( &__details::ctxErrorHandler );

    // Check for the GLX_ARB_create_context extension string and the function.
    // If either is not present, use GLX 1.3 context creation method.
    std::array<int, 5> context_attribs;
    context_attribs[0] = GLX_CONTEXT_MAJOR_VERSION_ARB;
    context_attribs[2] = GLX_CONTEXT_MINOR_VERSION_ARB;
    if ( configHandle->get<bool>( "use_maximum_context_version", true ) ) {
        // this parameters force X11 to use higher context among the possible
        context_attribs[1] = 3;
        context_attribs[3] = 0;
    } else {
        // or use user defined context version with 4.6 by default
        context_attribs[1] =
            configHandle->get<int>( "use_context_version_major", 4 );
        context_attribs[3] =
            configHandle->get<int>( "use_context_version_minor", 6 );
    }
    context_attribs[4] = GLX_CONTEXT_PROFILE_MASK_ARB;
    context_attribs[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
    context_attribs[6] = None;

    glContext_ = glXCreateContextAttribsARB( display_, bestFbc_, nullptr, True,
                                             context_attribs.data() );

    // error ocured
    if ( __details::ctxErrorOccurred && glContext_ ) {
        throw std::runtime_error( "can't create modern OpenGL context!" );
    }

    // Sync to ensure any errors generated are processed.
    XSync( display_, False );

    // Restore the original error handler
    XSetErrorHandler( oldHandler );

    if ( __details::ctxErrorOccurred || !glContext_ ) {
        throw std::runtime_error( "failed to create an OpenGL context" );
    }

    // Verifying that context is a direct context
    if ( !glXIsDirect( display_, glContext_ ) ) {
        log::info( "indirect GLX rendering context obtained" );
    } else {
        log::info( "direct GLX rendering context obtained" );
    }

    glXMakeCurrent( display_, window_, glContext_ );

    glEnable( GL_DEBUG_OUTPUT );
}

void RenderGL::setupDebugMessages() {
}

void RenderGL::displayRenderInfo() {
    std::print( R"foo(OpenGL context info:
    vendor - {}
    renderer - {}
    OpenGL version - {}
    GLSL version - {}
)foo",
                getGLVendorString(), getGLRendererString(),
                getGLVersionString(), getGLSLVendorString() );
}

void RenderGL::scene( const std::filesystem::path &path ) {
    scene_ = std::make_shared<gl::Scene>( path );
}

}  // namespace tire
