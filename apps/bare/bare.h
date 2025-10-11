
#ifndef __bare_window_h__
#define __bare_window_h__

#include <iostream>
#include <GLFW/glfw3.h>

struct BareWindow final {
    BareWindow() {
        if ( glfwInit() != GLFW_TRUE ) {
            std::cout << "glfwInit() return - GLFW_FALSE!"
                      << "\n";
            std::exit( 1 );
        }

        auto errorCallback = []( int, const char* err_str ) {
            std::cout << "GLFW Error: " << err_str << std::endl;
        };
        glfwSetErrorCallback( errorCallback );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
        glfwWindowHint( GLFW_DOUBLEBUFFER, GL_TRUE );
        glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

        window_ = glfwCreateWindow( width_, height_, "glfw", nullptr, nullptr );
    }
    ~BareWindow() {
        glfwDestroyWindow( window_ );
        glfwTerminate();
    }

private:
    GLFWwindow* window_{};
    GLFWmonitor* monitor_{};

    // window properties
    int posx_{};
    int posy_{};
    int width_{};
    int height_{};
};

#endif
