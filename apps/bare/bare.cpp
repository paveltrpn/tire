
#include <memory>
#include <GLFW/glfw3.h>

#include "bare.h"

BareWindow::BareWindow() {
    if ( glfwInit() != GLFW_TRUE ) {
        std::cout << "glfwInit() return - GLFW_FALSE!"
                  << "\n";
        std::exit( 1 );
    }

    auto errorCallback = []( int, const char* err_str ) {
        std::cout << "GLFW Error: " << err_str << std::endl;
    };
    glfwSetErrorCallback( errorCallback );

    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

    window_ = glfwCreateWindow( width_, height_, "glfw", nullptr, nullptr );

    glfwSetWindowUserPointer( window_, this );

    glfwSetKeyCallback( window_, []( GLFWwindow* window, int key, int scancode,
                                     int action, int mods ) {
        const auto rndrHandle =
            static_cast<tire::RenderVK*>( glfwGetWindowUserPointer( window ) );

        rndrHandle->keyPressEvent( key );
        rndrHandle->keyReleaseEvent( key );

        if ( key == GLFW_KEY_ESCAPE ) {
            glfwSetWindowShouldClose( window, GLFW_TRUE );
        }
    } );

    glfwSetMouseButtonCallback( window_, []( GLFWwindow* window, int button,
                                             int action, int mods ) {
        const auto rndrHandle =
            static_cast<tire::RenderVK*>( glfwGetWindowUserPointer( window ) );

        rndrHandle->mouseButtonPressEvent( button );
        rndrHandle->mouseButtonReleaseEvent( button );
    } );

    glfwSetCursorPosCallback( window_, []( GLFWwindow* window, double posX,
                                           double posY ) {
        const auto rndrHandle =
            static_cast<tire::RenderVK*>( glfwGetWindowUserPointer( window ) );

        // rndrHandle->mouseOffsetEvent( posX, posY );
    } );

    glfwSetCursorEnterCallback( window_, []( GLFWwindow* window, int entered ) {

    } );

    // NOTE: use native X11 window handles but there is no
    // X11 related definitions in code and no X11 headers indluded.
    const auto nWindow_ = glfwGetX11Window( window_ );
    const auto display_ = glfwGetX11Display();

    context_ = std::make_unique<tire::vk::ContextBare>( display_, nWindow_ );

    context_->init();

    render_ = std::make_unique<tire::RenderVK>();
    render_->init( context_.get() );

    context_->makeFrames( render_->renderPass() );

    render_->scene( "/mnt/main/code/tire/assets/m01.json" );
}

BareWindow::~BareWindow() {
    glfwDestroyWindow( window_ );
    glfwTerminate();
}

auto BareWindow::loop() -> void {
    while ( true ) {
        glfwPollEvents();

        render_->preFrame();

        context_->renderCommandBegin( currentFrame_, render_->renderPass() );

        render_->frame( context_->getDrawCommandBuffer( currentFrame_ ) );

        context_->renderCommandEnd( currentFrame_ );

        render_->postFrame();
        render_->swapBuffers();

        currentFrame_ = ( currentFrame_ + 1 ) % context_->framesCount();
    }
}
