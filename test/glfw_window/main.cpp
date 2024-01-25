
#include <iostream>
#include <format>
#include <print>
#include <memory>

import screen;

int main(int argc, char** argv) {
    std::print("test GLFW window\n");

    auto sc = std::make_unique<tire::X11Screen>();

    sc->init(tire::RenderType::SF);
    auto rd = sc->getRender();

    rd->run();

    return 0;
}
