
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <array>

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_RENDER_CPP{ true };
#include "io/factory.h"

#include "render.h"

namespace tire {

using namespace std::placeholders;

Render::Render() {
    if ( !tire::Config::instance() ) {
        throw std::runtime_error( "instantiate config first!!!" );
    }

    openDisplay();
    configureX11();

    loop_ = static_cast<uv_loop_t *>( malloc( sizeof( uv_loop_t ) ) );
    {
        const auto res = uv_loop_init( loop_ );
        if ( res != 0 ) {
            throw std::runtime_error(
                std::format( "uv loop init failed with code {}", res ) );
        } else {
            log::debug<DEBUG_OUTPUT_RENDER_CPP>(
                "Render === uv loop init success" );
        }
    }

    // IO Factory singleton explicit instatiation
    new io::Factory{ loop_ };

    uv_idle_init( loop_, &idler_ );

    // Every libuv handle has a void* data field. Here’s how you use it:
    idler_.data = this;

    {
        const auto res = uv_idle_start( &idler_, loop );
        if ( res != 0 ) {
            throw std::runtime_error(
                std::format( "uv idle start failed with code {}", res ) );
        } else {
            log::debug<DEBUG_OUTPUT_RENDER_CPP>( "Render === uv idle started" );
        }
    }
}

Render::~Render() {
    uv_loop_close( loop_ );
    free( loop_ );

    XDestroyWindow( display_, window_ );
    XFreeColormap( display_, colorMap_ );
    XCloseDisplay( display_ );
}

void Render::openDisplay() {
    display_ = XOpenDisplay( nullptr );

    if ( !display_ ) {
        throw std::runtime_error( "failed to open X display" );
    } else {
        log::info( "Render === open X11 display" );
    }
}

bool Render::isExtensionSupported( const char *extList,
                                   const char *extension ) {
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

void Render::configureX11() {
    log::info( "Render === begin configure X11..." );

    // Get a matching FB config
    constexpr std::array<int, 23> visual_attribs = {
        GLX_X_RENDERABLE, True, GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24, GLX_STENCIL_SIZE, 8, GLX_DOUBLEBUFFER, True,
        // GLX_SAMPLE_BUFFERS  , 1,
        // GLX_SAMPLES         , 4,
        None };

    int fbcount;
    GLXFBConfig *fbc = glXChooseFBConfig( display_, DefaultScreen( display_ ),
                                          visual_attribs.data(), &fbcount );
    if ( !fbc ) {
        throw std::runtime_error( "failed to retrieve a framebuffer config\n" );
    }

    // Pick the FB config/visual with the most samples per pixel
    log::info( "Render === getting XVisualInfos..." );
    int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

    for ( auto i = 0; i < fbcount; ++i ) {
        XVisualInfo *vi = glXGetVisualFromFBConfig( display_, fbc[i] );
        if ( vi ) {
            int samp_buf, samples;
            glXGetFBConfigAttrib( display_, fbc[i], GLX_SAMPLE_BUFFERS,
                                  &samp_buf );
            glXGetFBConfigAttrib( display_, fbc[i], GLX_SAMPLES, &samples );

            // log::info(
            // "matching fbconfig {}, visual ID {}: SAMPLE_BUFFERS = {},"
            // " SAMPLES = {}",
            // i,
            // vi->visualid,  // TODO: need to be HEX
            // samp_buf, samples );

            if ( best_fbc < 0 || ( samp_buf && samples > best_num_samp ) )
                best_fbc = i, best_num_samp = samples;
            if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
                worst_fbc = i, worst_num_samp = samples;
        }
        XFree( vi );
    }

    // choose best framebuffer config
    bestFbc_ = fbc[best_fbc];

    // Be sure to free the FBConfig list allocated by glXChooseFBConfig()
    XFree( static_cast<void *>( fbc ) );

    // Get a visual
    XVisualInfo *vi = glXGetVisualFromFBConfig( display_, bestFbc_ );
    log::info( "Render === chosen visual ID: {}", vi->visualid );

    // create colormap
    XSetWindowAttributes swa;
    swa.colormap = colorMap_ = XCreateColormap(
        display_, RootWindow( display_, vi->screen ), vi->visual, AllocNone );
    swa.background_pixmap = None;
    swa.border_pixel = 0;
    swa.event_mask = StructureNotifyMask;

    // create window
    auto cptr = Config::instance();
    posx_ = cptr->get<int>( "window_pos_x", 100 );
    posy_ = cptr->get<int>( "window_pos_y", 100 );
    width_ = cptr->get<int>( "window_width", 320 );
    height_ = cptr->get<int>( "window_height", 240 );

    window_ =
        XCreateWindow( display_, RootWindow( display_, vi->screen ), 0, 0,
                       width_, height_, 0, vi->depth, InputOutput, vi->visual,
                       CWBorderPixel | CWColormap | CWEventMask, &swa );

    if ( !window_ ) {
        throw std::runtime_error( "failed to create window" );
    } else {
        log::info( "Render === X11 window created" );
    }

    // Forbid to resize window by borders drag
    XSizeHints *size_hints = XAllocSizeHints();
    size_hints->flags = PMinSize | PMaxSize;
    size_hints->min_width = size_hints->max_width = width_;
    size_hints->min_height = size_hints->max_height = height_;
    XSetWMNormalHints( display_, window_, size_hints );
    XFree( size_hints );

    XFree( vi );

    XStoreName(
        display_, window_,
        cptr->get<std::string>( "application_name", "default" ).c_str() );
    XMapWindow( display_, window_ );

    XSelectInput( display_, window_, KeyPressMask | KeyReleaseMask );
}

void Render::loop( uv_idle_t *handle ) {
    // Retrive "this" pointer previously saved in uv_idle_t->data member
    auto self = static_cast<Render *>( handle->data );
    while ( XPending( self->display_ ) ) {
        XEvent event;
        XNextEvent( self->display_, &event );
        if ( event.type == KeyPress ) {
            auto keyEventCode = event.xkey.keycode;
            switch ( keyEventCode ) {
                case 9: {  // == ESCAPE
                    // Just stop render loop, not whole uv event loop
                    uv_idle_stop( handle );

                    // This will stop uv event loop and may
                    // cause uv_run() return code != 1 if
                    // not all event pendings got finished
                    uv_stop( self->loop_ );

                    self->run_ = false;

                    break;
                }
                case 38: {  // == 'a'
                    break;
                }
                default: {
                    break;
                }
            }
            // Force immidiate output to stdout
            std::cout << std::flush;
        } else if ( event.type == KeyRelease ) {
            auto keyEventCode = event.xkey.keycode;
            switch ( keyEventCode ) {
                default: {
                    break;
                }
            }
        }
    }
    self->preFrame();
    self->frame();
    self->postFrame();
    self->swapBuffers();
}

void Render::run() {
    preLoop();

    const auto res = uv_run( loop_, UV_RUN_DEFAULT );
    if ( res != 0 ) {
        log::warning( "Render === uv run end with code {}", res );
    }

    postLoop();
}

}  // namespace tire
