
#include <iostream>
#include <format>
#include <print>
#include <memory>

#include "subject.h"

#include "render/Render.h"
#include "render/RenderLegacyGL.h"
#include "config/Config.h"
#include "geometry/Point.h"
#include "geometry/Node.h"

#include "spdlog/spdlog.h"

int main(int argc, char **argv) {
    tire::Config config;
    try {
        config = tire::Config{ std::filesystem::path{ "../test/window/config.json" } };
    } catch (const std::exception &e) {
        spdlog::critical("caught exception: {}", e.what());
        return 0;
    }

    std::unique_ptr<tire::Render> rndr;
    try {
        rndr = std::make_unique<tire::RenderLegacyGL>(config);
    } catch (const std::exception &e) {
        spdlog::critical("caught exception: {}", e.what());
        return 0;
    }

    rndr->displayRenderInfo();

    initSubject(rndr.get());

    rndr->run();

    return 0;
}
