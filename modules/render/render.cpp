
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "spdlog/spdlog.h"

#include "render.h"

namespace tire
{

Render::Render() {
    if ( !tire::Config::instance() ) {
        spdlog::critical( "instantiate config first!!!" );
        throw std::runtime_error( "instantiate config first!!!" );
    }

    openDisplay();
    configureX11();
}

Render::~Render() {
    XDestroyWindow( display_, window_ );
    XFreeColormap( display_, colorMap_ );
    XCloseDisplay( display_ );
}

void Render::openDisplay() {
    display_ = XOpenDisplay( nullptr );

    if ( !display_ ) {
        throw std::runtime_error( "failed to open X display" );
    }
}

bool Render::isExtensionSupported( const char *extList, const char *extension ) {
    const char *start;
    const char *where, *terminator;

    /* Extension names should not have spaces. */
    where = strchr( extension, ' ' );
    if ( where || *extension == '\0' )
        return false;

    /* It takes a bit of care to be fool-proof about parsing the
       OpenGL extensions string. Don't be fooled by sub-strings,
       etc. */
    for ( start = extList;; ) {
        where = strstr( start, extension );

        if ( !where )
            break;

        terminator = where + strlen( extension );

        if ( where == start || *( where - 1 ) == ' ' )
            if ( *terminator == ' ' || *terminator == '\0' )
                return true;

        start = terminator;
    }

    return false;
}

void Render::configureX11() {
    // Get a matching FB config
    constexpr int visual_attribs[] = { GLX_X_RENDERABLE, True, GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, GLX_RENDER_TYPE,
                                       GLX_RGBA_BIT, GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR, GLX_RED_SIZE, 8, GLX_GREEN_SIZE,
                                       8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 8, GLX_DEPTH_SIZE, 24, GLX_STENCIL_SIZE, 8,
                                       GLX_DOUBLEBUFFER, True,
                                       // GLX_SAMPLE_BUFFERS  , 1,
                                       // GLX_SAMPLES         , 4,
                                       None };

    int fbcount;
    GLXFBConfig *fbc = glXChooseFBConfig( display_, DefaultScreen( display_ ), visual_attribs, &fbcount );
    if ( !fbc ) {
        throw std::runtime_error( "failed to retrieve a framebuffer config\n" );
    }

    // Pick the FB config/visual with the most samples per pixel
    spdlog::info( "getting XVisualInfos" );
    int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

    for ( auto i = 0; i < fbcount; ++i ) {
        XVisualInfo *vi = glXGetVisualFromFBConfig( display_, fbc[i] );
        if ( vi ) {
            int samp_buf, samples;
            glXGetFBConfigAttrib( display_, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
            glXGetFBConfigAttrib( display_, fbc[i], GLX_SAMPLES, &samples );

            spdlog::info( "matching fbconfig {}, visual ID {}: SAMPLE_BUFFERS = {},"
                          " SAMPLES = {}",
                          i,
                          vi->visualid, // TODO: need to be HEX
                          samp_buf, samples );

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
    XFree( fbc );

    // Get a visual
    XVisualInfo *vi = glXGetVisualFromFBConfig( display_, bestFbc_ );
    spdlog::info( "chosen visual ID = {}", vi->visualid );

    // create colormap
    XSetWindowAttributes swa;
    swa.colormap = colorMap_ = XCreateColormap( display_, RootWindow( display_, vi->screen ), vi->visual, AllocNone );
    swa.background_pixmap = None;
    swa.border_pixel = 0;
    swa.event_mask = StructureNotifyMask;

    // create window

    auto cptr = Config::instance();
    posx_ = cptr->get<int>( "window_pos_x", 100 );
    posy_ = cptr->get<int>( "window_pos_y", 100 );
    width_ = cptr->get<int>( "window_width", 320 );
    height_ = cptr->get<int>( "window_height", 240 );

    window_ = XCreateWindow( display_, RootWindow( display_, vi->screen ), posx_, posy_, width_, height_, 0, vi->depth,
                             InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa );

    if ( !window_ ) {
        throw std::runtime_error( "failed to create window" );
    }

    XFree( vi );

    XStoreName( display_, window_, cptr->get<std::string>( "application_name", "default" ).c_str() );
    XMapWindow( display_, window_ );
}

void Render::run() {
    if ( camera_ == nullptr ) {
        spdlog::critical( "how to render without camera???" );
        return;
    }

    if ( renderList_.empty() ) {
        spdlog::critical( "nothing to render!!!" );
        return;
    }

    initMainLoop();

    XSelectInput( display_, window_, KeyPressMask | KeyReleaseMask );
    while ( run_ ) {
        while ( XPending( display_ ) ) {
            XEvent KeyEvent;
            XNextEvent( display_, &KeyEvent );
            if ( KeyEvent.type == KeyPress ) {
                auto keyEventCode = KeyEvent.xkey.keycode;
                switch ( keyEventCode ) {
                case 9: { // == ESCAPE
                    run_ = false;
                    break;
                }
                default:
                    break;
                }

            } else if ( KeyEvent.type == KeyRelease ) {
                auto keyEventCode = KeyEvent.xkey.keycode;
                switch ( keyEventCode ) {
                default:
                    break;
                }
            }
        }
        preFrame();

        frame();

        postFrame();
        swapBuffers();
    }
}

} // namespace tire
