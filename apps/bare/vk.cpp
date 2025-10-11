
#include <memory>
#include <filesystem>

#include "log/log.h"
#include "render/rendervk.h"

import config;

int main( int argc, char** argv ) {
    std::unique_ptr<tire::Render> rndr;
    try {
        new tire::Config{ "assets/config.json" };

        rndr = std::make_unique<tire::RenderVK>();
        // rndr->displayRenderInfo();

        auto configPtr = tire::Config::instance();

        rndr->scene( configPtr->getBasePath() / "assets" / "m01.json" );
        rndr->run();
    } catch ( const std::exception& e ) {
        tire::log::error( "caught exception: {}", e.what() );
        return 1;
    }

    return 0;
}
