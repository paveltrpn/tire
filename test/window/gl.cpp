
#include <memory>
#include <filesystem>

#include "log/log.h"
#include "config/config.h"
#include "render/gl/rendergl.h"

int main( int argc, char **argv ) {
    std::unique_ptr<tire::Render> rndr;
    try {
        new tire::Config{ "test/window/config.json" };

        rndr = std::make_unique<tire::RenderGL>();
        // rndr->displayRenderInfo();

        auto configPtr = tire::Config::instance();

        rndr->scene( configPtr->getBasePath() / "assets" / "scene.json" );
        rndr->run();
    } catch ( const std::exception &e ) {
        tire::log::error( "caught exception: {}", e.what() );
        return 1;
    }

    return 0;
}
