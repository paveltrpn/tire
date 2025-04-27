

#include <GL/gl.h>
#include <GL/glx.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <iostream>
#include <format>
#include <print>
#include <map>

#include "log/log.h"
#include "render/gl/functions.h"
static constexpr bool DEBUG_OUTPUT_RENDERGL_CPP{ true };

#include "render/gl/screen_string.h"
#include "rendergl.h"

import config;

namespace tire {

namespace __details {
[[maybe_unused]] static void GLAPIENTRY MessageCallback(
    GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
    const GLchar *message, const void *userParam ) {
    std::cout << std::format(
        "GL CALLBACK: {} type = {}, severity = {}, message = {}\n",
        ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, severity,
        message );
}

static bool ctxErrorOccurred = false;
[[maybe_unused]] static int ctxErrorHandler( Display *dpy, XErrorEvent *ev ) {
    ctxErrorOccurred = true;
    return 0;
}

static bool isExtensionSupported( const char *extList, const char *extension ) {
    const char *start;
    const char *where, *terminator;

    /* Extension names should not have spaces. */
    where = strchr( extension, ' ' );
    if ( where || *extension == '\0' ) return false;

    /* It takes a bit of care to be fool-proof about parsing the
OpenGL extensions string. Don't be fooled by sub-strings,
etc. */
    for ( start = extList;; ) {
        where = strstr( start, extension );

        if ( !where ) break;

        terminator = where + strlen( extension );

        if ( where == start || *( where - 1 ) == ' ' )
            if ( *terminator == ' ' || *terminator == '\0' ) return true;

        start = terminator;
    }

    return false;
}

}  // namespace __details

using namespace tire::gl;

RenderGL::RenderGL()
    : Render{} {
    try {
        checkGlxVersion();
        initGlxExtensions();
        configureGl();
        gl::init();
    } catch ( std::runtime_error &e ) {
        throw std::runtime_error( std::format( "{}", e.what() ) );
    }

    auto configHandle = Config::instance();
    if ( configHandle->get<bool>( "enable_vsync" ) ) {
        setSwapInterval( 1 );
    } else {
        setSwapInterval( 0 );
    }

    if ( configHandle->get<bool>( "enable_debug_output" ) ) {
        glEnable( GL_DEBUG_OUTPUT );
        glDebugMessageCallback( &__details::MessageCallback, nullptr );
    }

    screenString_ =
        std::make_unique<ScreenString>( "RobotoMono-1024-512-32-64.tga" );

    // Framebuffer / Renderbuffer initialization

    glGenFramebuffers( 1, &framebuffer_.frambufferObject_ );
    glBindFramebuffer( GL_FRAMEBUFFER, framebuffer_.frambufferObject_ );

    glGenTextures( 1, &framebuffer_.frameTexture_ );
    glBindTexture( GL_TEXTURE_2D, framebuffer_.frameTexture_ );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, nullptr );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                            framebuffer_.frameTexture_, 0 );
    glBindTexture( GL_TEXTURE_2D, 0 );

    glGenRenderbuffers( 1, &framebuffer_.renderbufferObject_ );
    glBindRenderbuffer( GL_RENDERBUFFER, framebuffer_.renderbufferObject_ );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_,
                           height_ );

    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                               GL_RENDERBUFFER,
                               framebuffer_.renderbufferObject_ );

    if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) !=
         GL_FRAMEBUFFER_COMPLETE ) {
        log::error( " RenderGL === Ffamebuffer is not complete!" );
    }

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

    glGenVertexArrays( 1, &framebuffer_.array_ );
    glGenBuffers( 2, framebuffer_.buffers_.data() );

    // Make framebuffer quad
    glBindVertexArray( framebuffer_.array_ );

    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, framebuffer_.buffers_[0] );
    glBufferData( GL_ARRAY_BUFFER,
                  framebuffer_.frameQuadCrds.size() * 3 * sizeof( float ),
                  framebuffer_.frameQuadCrds.data(), GL_DYNAMIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

    glEnableVertexAttribArray( 1 );
    glBindBuffer( GL_ARRAY_BUFFER, framebuffer_.buffers_[1] );
    glBufferData( GL_ARRAY_BUFFER,
                  framebuffer_.frameQuadTexc.size() * 2 * sizeof( float ),
                  framebuffer_.frameQuadTexc.data(), GL_DYNAMIC_DRAW );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, nullptr );

    // Make frameboffer shader program
    frameProgram_.init( "frameProgram" );
}

RenderGL::~RenderGL() {
    glDeleteFramebuffers( 1, &framebuffer_.frambufferObject_ );
    glDeleteRenderbuffers( 1, &framebuffer_.renderbufferObject_ );
    glDeleteTextures( 1, &framebuffer_.frameTexture_ );

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

    if ( !__details::isExtensionSupported( glxExts,
                                           "GLX_ARB_create_context" ) ||
         !glXCreateContextAttribsARB ) {
        throw std::runtime_error(
            "extension glXCreateContextAttribsARB not supported!" );
    }

    glXSwapIntervalEXT = (glXSwapIntervalEXTProc)glXGetProcAddressARB(
        (const GLubyte *)"glXSwapIntervalEXT" );

    if ( !__details::isExtensionSupported( glxExts, "GLX_EXT_swap_control" ) ||
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

    __details::ctxErrorOccurred = false;
    int ( *oldHandler )( Display *, XErrorEvent * ) =
        XSetErrorHandler( &__details::ctxErrorHandler );

    // Check for the GLX_ARB_create_context extension string and the function.
    // If either is not present, use GLX 1.3 context creation method.
    std::array<int, 6> context_attribs;
    context_attribs[0] = GLX_CONTEXT_MAJOR_VERSION_ARB;
    context_attribs[1] = 4;
    context_attribs[2] = GLX_CONTEXT_MINOR_VERSION_ARB;
    context_attribs[3] = 6;
    context_attribs[4] = GLX_CONTEXT_PROFILE_MASK_ARB;
    context_attribs[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
    context_attribs[6] = None;

    glContext_ = glXCreateContextAttribsARB( display_, bestFbc_, nullptr, True,
                                             context_attribs.data() );

    // error ocured
    if ( __details::ctxErrorOccurred && glContext_ ) {
        throw std::runtime_error( "can't create OpenGL context!" );
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
