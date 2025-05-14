
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xfixes.h>

#include <array>
#include <print>
#include <coroutine>

#include "algebra/vector3.h"
#include "log/log.h"
#include "scene/flycam.h"
static constexpr bool DEBUG_OUTPUT_RENDER_CPP{ true };

#include "render.h"

import config;
import event;
import io;

namespace tire {

using namespace std::placeholders;

Render::Render() {
    if ( !tire::Config::instance() ) {
        throw std::runtime_error( "instantiate config first!!!" );
    }
    
    // Crate X11 window
    configureX11();

    // Event Context singleton explicit instatiation
    new event::Context{};
}

Render::~Render() {
    XDestroyWindow( display_, window_ );
    XFreeColormap( display_, colorMap_ );
    XCloseDisplay( display_ );
}

void Render::configureX11() {
    log::info( "Render === begin configure X11..." );

    display_ = XOpenDisplay( nullptr );

    if ( !display_ ) {
        throw std::runtime_error( "failed to open X display" );
    } else {
        log::info( "Render === open X11 display" );
    }

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
    auto configHandle = Config::instance();
    posx_ = configHandle->get<int>( "window_pos_x", 100 );
    posy_ = configHandle->get<int>( "window_pos_y", 100 );
    width_ = configHandle->get<int>( "window_width", 320 );
    height_ = configHandle->get<int>( "window_height", 240 );

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

    XStoreName( display_, window_,
                configHandle->get<std::string>( "application_name", "default" )
                    .c_str() );
    XMapWindow( display_, window_ );

    // Allow X11 propagate events to a window
    XSelectInput( display_, window_,
                  KeyPressMask | KeyReleaseMask | ButtonPressMask |
                      ButtonReleaseMask | PointerMotionMask );

    // Move newly created window to initial window position
    XMoveResizeWindow( display_, window_, posx_, posy_, width_, height_ );

    // Handle cursor visibility at startup
    if ( holdMouse_ ) {
        XFixesHideCursor( display_, window_ );
    }
}

void Render::loop( uv_timer_t *handle ) {
    // Retrive "this" pointer previously saved in uv_handle_t->data member
    auto self = static_cast<Render *>( handle->data );

    // XLib event loop
    while ( XPending( self->display_ ) ) {
        XEvent event;
        XNextEvent( self->display_, &event );
        if ( event.type == KeyPress ) {
            auto keyEventCode = event.xkey.keycode;

            // Handle key press
            self->keyPressEvent( event.xkey.keycode );

            // Handle escape button here for convinient access to uv timer handle
            switch ( keyEventCode ) {
                case 9: {  // == ESCAPE
                    // stop render loop
                    uv_timer_stop( handle );
                    event::Context::stop();
                    self->run_ = false;
                    break;
                }
                default: {
                    break;
                }
            }
            // Force immidiate output to stdout
            std::cout << std::flush;
        } else if ( event.type == KeyRelease ) {
            self->keyReleaseEvent( event.xkey.keycode );
        } else if ( event.type == ButtonPress ) {
            self->mouseButtonPressEvent( event.xbutton.button );
        } else if ( event.type == ButtonRelease ) {
            self->mouseButtonReleaseEvent( event.xbutton.button );
        } else if ( event.type == MotionNotify ) {
            if ( self->holdMouse_ )
                self->mouseOffsetEvent( event.xmotion.x, event.xmotion.y );
            else {
                self->mouseMoveEvent( event.xmotion.x, event.xmotion.y );
            }
        }
    }
    self->preFrame();
    self->frame();
    self->postFrame();
    self->swapBuffers();

    // Conditionally force mouse hold position.
    if ( self->holdMouse_ ) {
        XWarpPointer( self->display_, None, self->window_, 0, 0, 0, 0,
                      self->holdMouseX_, self->holdMouseY_ );
    }
}

void Render::run() {
    event::Context::render( this, Render::loop );
    preLoop();
    event::Context::run();
    postLoop();
}

void Render::keyPressEvent( unsigned int key ) {
    switch ( key ) {
        case 24: {  // == 'q'
            scene_->nextCamera();
            break;
        }
        case 25: {  // == 'w'
            scene_->camera().setMoveBit( FlycamMoveBits::FORWARD );
            break;
        }
        case 26: {  // == 'e'
            break;
        }
        case 27: {  // == 'r'
            break;
        }
        case 28: {  // == 't'
            break;
        }
        case 29: {  // == 'y'
            break;
        }
        case 30: {  // == 'u'
            break;
        }
        case 31: {  // == 'i'
            break;
        }
        case 32: {  // == 'o'
            break;
        }
        case 33: {  // == 'p'
            break;
        }
        case 34: {  // == '['
            break;
        }
        case 35: {  // == ']'
            break;
        }
        case 38: {  // == 'a'
            scene_->camera().setMoveBit( FlycamMoveBits::LEFT );
            break;
        }
        case 39: {  // == 's'
            scene_->camera().setMoveBit( FlycamMoveBits::BACKWARD );
            break;
        }
        case 40: {  // == 'd'
            scene_->camera().setMoveBit( FlycamMoveBits::RIGHT );
            break;
        }
        case 41: {  // == 'f'
            break;
        }
        case 42: {  // == 'g'
            // Window grab/release the mouse pointer
            holdMouse_ = !holdMouse_;
            if ( holdMouse_ ) {
                XFixesHideCursor( display_, window_ );
            } else {
                XFixesShowCursor( display_, window_ );
            }
            break;
        }
        case 43: {  // == 'h'
            break;
        }
        case 44: {  // == 'j'
            break;
        }
        case 45: {  // == 'k'
            break;
        }
        case 46: {  // == 'l'
            break;
        }
        case 47: {  // == ';'
            break;
        }
        case 48: {  // == '''
            break;
        }
        case 52: {  // == 'z'
            break;
        }
        default: {
            break;
        }
    }
}

void Render::keyReleaseEvent( unsigned int key ) {
    switch ( key ) {
        case 25: {  // == 'w'
            scene_->camera().unsetMoveBit( FlycamMoveBits::FORWARD );
            break;
        }
        case 38: {  // == 'a'
            scene_->camera().unsetMoveBit( FlycamMoveBits::LEFT );
            break;
        }
        case 39: {  // == 's'
            scene_->camera().unsetMoveBit( FlycamMoveBits::BACKWARD );
            break;
        }
        case 40: {  // == 'd'
            scene_->camera().unsetMoveBit( FlycamMoveBits::RIGHT );
            break;
        }
        default: {
            break;
        }
    }
}

void Render::mouseButtonPressEvent( unsigned int key ) {
}

void Render::mouseButtonReleaseEvent( unsigned int key ) {
}

void Render::mouseMoveEvent( unsigned int x, unsigned int y ) {
}

void Render::mouseOffsetEvent( unsigned int x, unsigned int y ) {
    const int xOffset = holdMouseX_ - x;
    const int yOffset = holdMouseY_ - y;

#define MOUSE_SENSIVITY 0.002
    scene_->camera().rotate( xOffset * MOUSE_SENSIVITY,
                             yOffset * MOUSE_SENSIVITY );
}

event::Task<void> Render::executeByTimeOut( long long timeout ) {
    co_await event::setTimeout( timeout );
    log::notice( "test coroutine timer trigger" );
};

event::Task<void> Render::watchFile( std::string path ) {
    bool isRenamed{ false };
    while ( !isRenamed ) {
        const auto res = co_await io::watchForFile( path );
        // Wait for uv_fs_event_stop() to cancel next callback call
        co_await event::setTimeout( 100 );
        log::notice( "file  \"{}\" changed with code {}", path,
                     static_cast<int>( res ) );
        if ( isRenamed = ( res == UV_RENAME ); isRenamed ) {
            log::notice( "file  \"{}\" have been renamed, stop watching",
                         path );
        }
    }
};

event::Task<void> Render::showMetrics() {
    while ( true ) {
        co_await event::setTimeout( 3000 );

        event::Context::metrics();

        const auto afd = timer_.averageFrameDuration();
        const auto [min, max] = timer_.minMaxFrameDuration();
        const auto fps = timer_.framesPerSecond();

        log::info( "frame duration = {}, min = {}, max = {}, fps = {:<3.2f}",
                   afd, min, max, fps );

        timer_.resetAccumulators();
    }
}

}  // namespace tire
