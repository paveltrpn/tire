
#include <memory>

#define SURFACE_X11
// #define SURFACE_WAYLAND

#ifdef SURFACE_X11
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#elifdef SURFACE_WAYLAND
#define GLFW_EXPOSE_NATIVE_WAYLAND
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <wayland-client.h>
#endif

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "window.h"

#include "config/config.h"
#include "context/context.h"
#include "render/rendervk.h"
#include "config/config.h"
#include "log/log.h"

BareWindow::BareWindow() {
    // Initialize Config sinleton.
    tire::Config::init( "config.json" );

    if ( glfwInit() != GLFW_TRUE ) {
        tire::log::fatal()( "glfw init faild!" );
    }

    glfwSetErrorCallback( []( int, const char *err_str ) -> void {
        //
        tire::log::error()( "GLFW Error: {}", err_str );
    } );

    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

    const auto width = tire::Config::instance().get<int>( "window_width" );
    const auto height = tire::Config::instance().get<int>( "window_height" );
    const auto posx = tire::Config::instance().get<int>( "window_pos_x" );
    const auto posy = tire::Config::instance().get<int>( "window_pos_y" );

    const auto windowTitle = tire::Config::instance().get<std::string>( "application_name" );
    window_ = glfwCreateWindow( width, height, windowTitle.c_str(), nullptr, nullptr );

    if ( !window_ ) {
        tire::log::fatal()( "glfw window create faild!" );
    }


#ifdef SURFACE_X11
            tire::log::info()( "glfw platform X11 is used!" );

            const auto display = glfwGetX11Display();
            const auto window = glfwGetX11Window( window_ );

            // Initialize Context sinleton.
            tire::Context::init( width, height, display, window );

            glfwSetWindowPos( window_, posx, posy );
#elifdef SURFACE_WAYLAND
            tire::log::info()( "glfw platform WAYLAND is used!" );

            const auto display = glfwGetWaylandDisplay();
            const auto surface = glfwGetWaylandWindow( window_ );

            // Initialize Context sinleton.
            tire::Context::init( width, height, display, surface );
#endif

    // Initialize render object.
    render_ = std::make_unique<tire::RenderVK>();

    glfwSetWindowUserPointer( window_, render_.get() );

    glfwSetKeyCallback( window_, []( GLFWwindow *window, int key, int scancode, int action, int mods ) -> void {
        const auto rndrHandle = static_cast<tire::RenderVK *>( glfwGetWindowUserPointer( window ) );

        if ( key == GLFW_KEY_G ) {
            if ( rndrHandle->holdMouse() ) {
                glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
            } else {
                glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
            }
        }

        if ( action == GLFW_PRESS ) {
            rndrHandle->keyPressEvent( key );
        }

        if ( action == GLFW_RELEASE ) {
            rndrHandle->keyReleaseEvent( key );
        }
    } );

    glfwSetMouseButtonCallback( window_, []( GLFWwindow *window, int button, int action, int mods ) -> void {
        const auto rndrHandle = static_cast<tire::RenderVK *>( glfwGetWindowUserPointer( window ) );

        if ( action == GLFW_PRESS ) {
            rndrHandle->mouseButtonPressEvent( button );
        }

        if ( action == GLFW_RELEASE ) {
            rndrHandle->mouseButtonReleaseEvent( button );
        }
    } );

#define WINDOW_HOLD_X 500.0
#define WINDOW_HOLD_Y 500.0

    glfwSetCursorPosCallback( window_, []( GLFWwindow *window, double posX, double posY ) -> void {
        const auto rndrHandle = static_cast<tire::RenderVK *>( glfwGetWindowUserPointer( window ) );

        if ( rndrHandle->holdMouse() ) {
            rndrHandle->mouseOffsetEvent( posX, posY, WINDOW_HOLD_X, WINDOW_HOLD_Y );
        }
    } );

    glfwSetCursorEnterCallback( window_, []( GLFWwindow *window, int entered ) -> void {

    } );
}

BareWindow::~BareWindow() {
    glfwDestroyWindow( window_ );
    glfwTerminate();
}

auto BareWindow::loop() -> void {
    render_->preLoop();

    while ( render_->run() ) {
        if ( render_->holdMouse() ) {
            glfwSetCursorPos( window_, WINDOW_HOLD_X, WINDOW_HOLD_Y );
        }

        glfwPollEvents();

        render_->frame();
    }

    glfwSetWindowShouldClose( window_, GLFW_TRUE );

    render_->postLoop();
}
