
#include <memory>
#include <format>

#include <GLFW/glfw3.h>

#include "config/config.h"
#include "bare.h"
#include "log/log.h"

BareWindow::BareWindow() {
    if ( glfwInit() != GLFW_TRUE ) {
        tire::log::fatal( "glfw init faild!" );
    }

    auto errorCallback = []( int, const char* err_str ) -> void {
        std::cout << std::format( "GLFW Error: {}\n", err_str );
    };

    glfwSetErrorCallback( errorCallback );

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

    glfwSetWindowPos( window_, posx, posy );

    glfwSetWindowUserPointer( window_, this );

    glfwSetKeyCallback( window_,
                        []( GLFWwindow* window, int key, int scancode,
                            int action, int mods ) -> void {
                            const auto rndrHandle =
                                static_cast<tire::RenderVK*>(
                                    glfwGetWindowUserPointer( window ) );

                            rndrHandle->keyPressEvent( key );
                            rndrHandle->keyReleaseEvent( key );

                            if ( key == GLFW_KEY_ESCAPE ) {
                                glfwSetWindowShouldClose( window, GLFW_TRUE );
                            }
                        } );

    glfwSetMouseButtonCallback(
        window_,
        []( GLFWwindow* window, int button, int action, int mods ) -> void {
            const auto rndrHandle = static_cast<tire::RenderVK*>(
                glfwGetWindowUserPointer( window ) );

            rndrHandle->mouseButtonPressEvent( button );
            rndrHandle->mouseButtonReleaseEvent( button );
        } );

    glfwSetCursorPosCallback(
        window_, []( GLFWwindow* window, double posX, double posY ) -> void {
            const auto rndrHandle = static_cast<tire::RenderVK*>(
                glfwGetWindowUserPointer( window ) );

            // rndrHandle->mouseOffsetEvent( posX, posY );
        } );

    glfwSetCursorEnterCallback( window_,
                                []( GLFWwindow* window, int entered ) -> void {

                                } );

    // NOTE: use native X11 window handles but there is no
    // X11 related definitions in code and no X11 headers indluded.
    const auto nWindow_ = glfwGetX11Window( window_ );
    const auto display_ = glfwGetX11Display();

    context_ = std::make_unique<tire::vk::ContextBare>( display_, nWindow_ );
    context_->init();
    context_->setViewportSize( width, height );

    render_ = std::make_unique<tire::RenderVK>();
    render_->init( context_.get() );

    const auto basePath = tire::Config::instance()->getBasePath();
    render_->scene( basePath.string() + "/assets/m01.json" );
}

BareWindow::~BareWindow() {
    glfwDestroyWindow( window_ );
    glfwTerminate();
}

auto BareWindow::loop() -> void {
    while ( true ) {
        glfwPollEvents();

        render_->preFrame();

        context_->renderCommandBegin( currentFrame_ );

        render_->frame( context_->getDrawCommandBuffer( currentFrame_ ) );

        context_->renderCommandEnd( currentFrame_ );

        render_->postFrame();
        render_->swapBuffers();

        currentFrame_ = ( currentFrame_ + 1 ) % context_->framesCount();
    }
}
