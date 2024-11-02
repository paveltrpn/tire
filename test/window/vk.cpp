
#include <memory>
#include <filesystem>

#include "log/log.h"

#include "subject.h"
#include "config/config.h"
#include "render/rendervk.h"

int main( int argc, char **argv ) {
    try {
        new tire::Config{ std::filesystem::path{ "../test/window/config.json" } };
    } catch ( const std::exception &e ) {
        tire::log::error( "caught exception: {}", e.what() );
        return 0;
    }

    std::unique_ptr<tire::Render> rndr;
    try {
        rndr = std::make_unique<tire::RenderVK>();
    } catch ( const std::exception &e ) {
        tire::log::error( "caught exception: {}", e.what() );
        return 0;
    }

    rndr->displayRenderInfo();

    initSubject( rndr.get() );

    rndr->run();

    return 0;
}
