
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

    uint32_t extensionCount;
    const char** glfwExtensions =
        glfwGetRequiredInstanceExtensions( &extensionCount );

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
    }
}
