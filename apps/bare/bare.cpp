
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

    nWindow_ = glfwGetX11Window( window_ );
    display_ = glfwGetX11Display();

    context_ = std::make_unique<tire::vk::Context>( display_, nWindow_ );

    context_->init();

    render_ = std::make_unique<tire::RenderVK>( context_.get() );

    render_->scene( "/mnt/main/code/tire/assets/m01.json" );

    // VkSurfaceKHR surface;
    // if ( glfwCreateWindowSurface( instance, window, nullptr, &surface ) !=
    //  VK_SUCCESS ) {
    // }
}

BareWindow::~BareWindow() {
    glfwDestroyWindow( window_ );
    glfwTerminate();
}

auto BareWindow::loop() -> void {
    while ( true ) {
        glfwPollEvents();

        render_->preFrame();
        render_->frame();
        render_->postFrame();
        render_->swapBuffers();
    }
}
