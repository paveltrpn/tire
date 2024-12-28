

#include <GL/glx.h>
#include <memory>
#include <string>
#include <iostream>
#include <format>
#include <print>
#include <map>

#include "log/log.h"
#include "rendergl.h"
#include "geometry/node.h"
#include "camera/camera.h"

#define ENABLE_DEBUG_OUTPUT true

namespace tire {

using namespace tire::gl;

RenderGL::RenderGL()
    : Render{} {
    checkGlxVersion();
    initGlxExtensions();
    configureGl();
    gl::init();
    setSwapInterval( 1 );
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

    __detail::ctxErrorOccurred = false;
    int ( *oldHandler )( Display *, XErrorEvent * ) =
        XSetErrorHandler( &__detail::ctxErrorHandler );

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
        // or use user defined context version with 3.3 by default
        context_attribs[1] =
            configHandle->get<int>( "use_context_version_major", 3 );
        context_attribs[3] =
            configHandle->get<int>( "use_context_version_minor", 3 );
    }
    context_attribs[4] = GLX_CONTEXT_PROFILE_MASK_ARB;
    context_attribs[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
    context_attribs[6] = None;

    glContext_ = glXCreateContextAttribsARB( display_, bestFbc_, nullptr, True,
                                             context_attribs.data() );

    // error ocured
    if ( __detail::ctxErrorOccurred && glContext_ ) {
        throw std::runtime_error( "can't create modern OpenGL context!" );
    }

    // Sync to ensure any errors generated are processed.
    XSync( display_, False );

    // Restore the original error handler
    XSetErrorHandler( oldHandler );

    if ( __detail::ctxErrorOccurred || !glContext_ ) {
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

void RenderGL::prepareShaders() {
    auto program = gl::Shader{};
    program.link(
        { { GL_VERTEX_SHADER,
            shaderSourcesManager_.getVertexShader( "basic_color" ) },
          { GL_FRAGMENT_SHADER,
            shaderSourcesManager_.getFragmentShader( "basic_color" ) } } );

    program.use();

    auto matrix = program.getUniformLocation( "matrix" );

    program.setMatrixUniform( matrix, GL_FALSE, camera_->getMatrix() );
    auto color = program.getUniformLocation( "color" );
    program.setVectorUniform( color, algebra::vector3f{ 0.9f, 0.2f, 0.5f } );

    log::debug<ENABLE_DEBUG_OUTPUT>( "mat: {}, col: {}", matrix, color );

    programs_.insert( std::pair{ gl::ShaderID::BASIC_COLOR, program } );
}

// =============================================================================
// ================ core render work ===========================================
// =============================================================================

void RenderGL::initMainLoop() {
    prepareShaders();

    gl::Shader basic_color = programs_[gl::ShaderID::BASIC_COLOR];

    basic_color.use();
};

void RenderGL::preFrame() {
    glViewport( 0, 0, width_, height_ );
    glClearColor( 0, 0.5, 1, 1 );
    glClear( GL_COLOR_BUFFER_BIT );

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );
}

void RenderGL::frame() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBindBuffer( GL_ARRAY_BUFFER, bufferObject_ );

    // glEnableVertexAttribArray( 0 );
    // glBindVertexArray( vertexObject_ );
    // glDrawElements( GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void *)0 );
    // glDisableVertexAttribArray( 0 );

    glBindVertexArray( vertexObject_ );
    glDrawArrays( GL_TRIANGLES, 0, 3 );
};

void RenderGL::postFrame() {
}

void RenderGL::swapBuffers() {
    glXSwapBuffers( display_, window_ );
};

// =============================================================================
// =============================================================================
// =============================================================================

void RenderGL::appendToRenderList( std::shared_ptr<tire::Node> node ) {
    renderList_.push_back( node );

    float positions[]{ 0.0f, 0.5f, 0.0f,  -0.5f, -0.5f,
                       0.0f, 0.5f, -0.5f, 0.0f };

    glGenVertexArrays( 1, &vertexObject_ );
    glBindVertexArray( vertexObject_ );
    glGenBuffers( 1, &bufferObject_ );
    glBindBuffer( GL_ARRAY_BUFFER, vertexObject_ );
    glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 9, &positions[0],
                  GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, false, 0, 0 );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

    // glGenBuffers( 1, &bufferObject_ );
    //

    // glBindBuffer( GL_ARRAY_BUFFER, bufferObject_ );
    // glBindVertexArray( vertexObject_ );
    // glBufferData( GL_ARRAY_BUFFER, node->verteciesArraySize(), nullptr,
    // GL_STATIC_DRAW );
    //position attribute
    // glVertexAttribPointer( 0, 3, GL_DOUBLE, GL_FALSE, 0, nullptr );
    // glEnableVertexAttribArray( 0 );

    // glGenBuffers( 1, &elementsObject_ );
    // glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, elementsObject_ );
    // glBufferData( GL_ELEMENT_ARRAY_BUFFER, node->indeciesArraySize(),
    // node->indeciesData(), GL_STATIC_DRAW );

    // glBindBuffer( GL_ARRAY_BUFFER, bufferObject_ );
    // glBindVertexArray( vertexObject_ );
    // glBufferSubData( GL_ARRAY_BUFFER, 0, node->verteciesArraySize(),
    //node->verteciesData() );
    // color attribute
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)(12 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    // texture coordinates attribute
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)(24 * sizeof(float)));
    // glEnableVertexAttribArray(2);
}

}  // namespace tire
