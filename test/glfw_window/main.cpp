
#include <iostream>
#include <format>
#include <print>
#include <memory>

import screen;

int main(int argc, char** argv) {
    std::print("test GLFW window\n");

    auto sc = std::make_unique<tire::GLFWScreen>("first");
    
    sc->setWindowPosX(500);
    sc->init(tire::RenderType::OPENGL);

    sc->run();

    return 0;
}
