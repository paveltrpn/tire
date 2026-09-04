module;

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

#include "event/eventemitter.h"
#include "render/rendervk.h"
#include "context/context.h"

#include "config/config.h"
#include "log/log.h"

export module barewindow : barewindow;

import program;
import : testscene;

export struct BareWindow final {
    BareWindow() {
        // Initialize GlobalEventEmitter singleton.
        tire::GlobalEventEmitter::init();
        // Initialize Config singleton.
        tire::Config::init( "assets/config.json" );

        if ( glfwInit() != GLFW_TRUE ) {
            tire::log::fatal()( "glfw init faild!" );
        }

        glfwSetErrorCallback( []( int, const char* err_str ) -> void {
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
        _window = glfwCreateWindow( width, height, windowTitle.c_str(), nullptr, nullptr );

        if ( !_window ) {
            tire::log::fatal()( "glfw window create faild!" );
        }

#ifdef SURFACE_X11
        tire::log::info()( "glfw platform X11 is used!" );

        const auto display = glfwGetX11Display();
        const auto window = glfwGetX11Window( _window );

        // Initialize Context sinleton.
        tire::Context::init( width, height, display, window );

        glfwSetWindowPos( _window, posx, posy );
#elifdef SURFACE_WAYLAND
        tire::log::info()( "glfw platform WAYLAND is used!" );

        const auto display = glfwGetWaylandDisplay();
        const auto surface = glfwGetWaylandWindow( window_ );

        // Initialize Context sinleton.
        tire::Context::init( width, height, display, surface );
#endif

        // Initialize render object.
        _render = std::make_shared<tire::RenderVK>();
        // Subscribe.
        tire::GlobalEventEmitter::instance().attach( _render );

        glfwSetWindowUserPointer( _window, _render.get() );

        glfwSetKeyCallback( _window, []( GLFWwindow* window, int key, int scancode, int action, int mods ) -> void {
            const auto rndrHandle = static_cast<tire::RenderVK*>( glfwGetWindowUserPointer( window ) );

            if ( action == GLFW_PRESS ) {
                auto keyPressEvent = std::make_shared<tire::EventKey>( tire::KeyAction::PRESS, key );
                tire::GlobalEventEmitter::instance().notify( std::move( keyPressEvent ) );
            }

            if ( action == GLFW_RELEASE ) {
                auto keyPressEvent = std::make_shared<tire::EventKey>( tire::KeyAction::RELEASE, key );
                tire::GlobalEventEmitter::instance().notify( std::move( keyPressEvent ) );
            }
        } );

        glfwSetMouseButtonCallback( _window, []( GLFWwindow* window, int button, int action, int mods ) -> void {
            const auto rndrHandle = static_cast<tire::RenderVK*>( glfwGetWindowUserPointer( window ) );

            if ( action == GLFW_PRESS ) {
            }

            if ( action == GLFW_RELEASE ) {
            }
        } );

#define WINDOW_HOLD_X 500.0
#define WINDOW_HOLD_Y 500.0

        glfwSetCursorPosCallback( _window, []( GLFWwindow* window, double posX, double posY ) -> void {
            const auto rndrHandle = static_cast<tire::RenderVK*>( glfwGetWindowUserPointer( window ) );

            // if ( rndrHandle->holdMouse() ) {
            //     rndrHandle->mouseOffsetEvent( posX, posY, WINDOW_HOLD_X, WINDOW_HOLD_Y );
            // }
        } );

        glfwSetCursorEnterCallback( _window, []( GLFWwindow* window, int entered ) -> void {

        } );
    }

    BareWindow( const BareWindow& other ) = delete;
    BareWindow( BareWindow&& other ) = delete;
    auto operator=( const BareWindow& other ) -> BareWindow& = delete;
    auto operator=( BareWindow&& other ) -> BareWindow& = delete;

    ~BareWindow() {
        tire::Context::instance().releaseContext();

        glfwDestroyWindow( _window );
        glfwTerminate();
    }

    auto loop() -> void {
        auto ts = testScene();

        _render->attachScene( std::move( ts ) );

        _render->preLoop();

        while ( _render->isRun() ) {
            // if ( render_->holdMouse() ) {
            //     glfwSetCursorPos( window_, WINDOW_HOLD_X, WINDOW_HOLD_Y );
            // }

            glfwPollEvents();

            _render->frame();
        }

        glfwSetWindowShouldClose( _window, GLFW_TRUE );

        _render->postLoop();
    }

private:
    GLFWwindow* _window{};
    GLFWmonitor* _monitor{};

    // window properties
    int _posx{};
    int _posy{};
    int _width{ 1024 };
    int _height{ 1024 };

    std::shared_ptr<tire::RenderVK> _render;
};
