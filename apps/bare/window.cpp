
#include <memory>
#include <format>
#include <iostream>

#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "window.h"

#ifdef SURFACE_X11
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>
#elifdef SURFACE_WAYLAND
#define GLFW_EXPOSE_NATIVE_WAYLAND
#include <GLFW/glfw3native.h>
#include <wayland-client.h>
#endif

#include "config/config.h"

import log;
import render;

BareWindow::BareWindow() {
    if ( glfwInit() != GLFW_TRUE ) {
        tire::log::fatal( "glfw init faild!" );
    }

    glfwSetErrorCallback(
        []( int, const char* err_str ) -> void { std::cout << std::format( "GLFW Error: {}\n", err_str ); } );

    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

    const auto configHandle = tire::Config::instance();
    const auto width = configHandle->get<int>( "window_width" );
    const auto height = configHandle->get<int>( "window_height" );
    const auto posx = configHandle->get<int>( "window_pos_x" );
    const auto posy = configHandle->get<int>( "window_pos_y" );

    window_ = glfwCreateWindow( width, height, "glfw", nullptr, nullptr );

    if ( !window_ ) {
        tire::log::fatal( "glfw window create faild!" );
    }

    glfwSetWindowUserPointer( window_, this );

    glfwSetKeyCallback( window_, []( GLFWwindow* window, int key, int scancode, int action, int mods ) -> void {
        const auto rndrHandle = static_cast<tire::RenderVK*>( glfwGetWindowUserPointer( window ) );

        rndrHandle->keyPressEvent( key );
        rndrHandle->keyReleaseEvent( key );

        if ( key == GLFW_KEY_ESCAPE ) {
            glfwSetWindowShouldClose( window, GLFW_TRUE );
        }
    } );

    glfwSetMouseButtonCallback( window_, []( GLFWwindow* window, int button, int action, int mods ) -> void {
        const auto rndrHandle = static_cast<tire::RenderVK*>( glfwGetWindowUserPointer( window ) );

        rndrHandle->mouseButtonPressEvent( button );
        rndrHandle->mouseButtonReleaseEvent( button );
    } );

    glfwSetCursorPosCallback( window_, []( GLFWwindow* window, double posX, double posY ) -> void {
        const auto rndrHandle = static_cast<tire::RenderVK*>( glfwGetWindowUserPointer( window ) );

        // rndrHandle->mouseOffsetEvent( posX, posY );
    } );

    glfwSetCursorEnterCallback( window_, []( GLFWwindow* window, int entered ) -> void {

    } );

    const auto platform = glfwGetPlatform();

    switch ( platform ) {
        case GLFW_PLATFORM_NULL: {
            tire::log::fatal( "glfw platform undefined!" );
            break;
        }
        case GLFW_PLATFORM_WIN32: {
            tire::log::fatal( "glfw platform win32 not supported!" );
            break;
        }
        case GLFW_PLATFORM_COCOA: {
            tire::log::fatal( "glfw platform cocoa not supported!" );
            break;
        }
        case GLFW_PLATFORM_X11: {
            tire::log::info( "glfw platform X11 is used!" );

#ifdef SURFACE_X11
            context_ = std::make_unique<tire::ContextBare>(
                /*"VK_KHR_xlib_surface"*/ );

            context_->makeInstance( "VK_KHR_xlib_surface" );

            const auto window = glfwGetX11Window( window_ );
            const auto display = glfwGetX11Display();
            context_->makeXlibSurface( display, window );
#endif
            glfwSetWindowPos( window_, posx, posy );
            break;
        }
        case GLFW_PLATFORM_WAYLAND: {
            tire::log::info( "glfw platform WAYLAND is used!" );

#ifdef SURFACE_WAYLAND
            context_ = std::make_unique<tire::vk::ContextBare>(
                /*"VK_KHR_wayland_surface"*/ );

            context_->makeInstance( "VK_KHR_wayland_surface" );
            const auto surface = glfwGetWaylandWindow( window_ );
            const auto display = glfwGetWaylandDisplay();
            context_->makeWaylandSurface( display, surface );
#endif

            break;
        }
        default: {
            break;
        }
    }
    context_->setViewportSize( width, height );
    context_->init();

    render_ = std::make_unique<tire::RenderVK>();
    render_->init( context_.get() );

    const auto basePath = configHandle->getBasePath();
    render_->scene( basePath.string() + "/assets/m01.json" );
}

BareWindow::~BareWindow() {
    glfwDestroyWindow( window_ );
    glfwTerminate();
}

auto BareWindow::loop() -> void {
    render_->preLoop();

    while ( run_ ) {
        glfwPollEvents();

        render_->preFrame();

        context_->renderCommandBegin( currentFrame_ );

        render_->frame( context_->getDrawCommandBuffer( currentFrame_ ) );

        context_->renderCommandEnd( currentFrame_ );

        render_->postFrame();
        render_->swapBuffers();

        currentFrame_ = ( currentFrame_ + 1 ) % context_->framesCount();
    }

    render_->postLoop();
}
