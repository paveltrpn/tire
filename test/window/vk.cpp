
#include <memory>
#include <filesystem>
#include <string>
#include "log/log.h"

#include "scene.h"
#include "config/config.h"
#include "render/vk/rendervk.h"

int main( int argc, char **argv ) {
    try {
        new tire::Config{
            std::filesystem::path{ "../test/window/config.json" } };
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

    // rndr->displayRenderInfo();

    initScene( rndr.get() );

    {
        auto configPtr = tire::Config::instance();
        auto width =
            static_cast<float>( configPtr->get<int>( "window_width" ) );
        auto height =
            static_cast<float>( configPtr->get<int>( "window_height" ) );
        auto camera = std::make_shared<tire::camera::Perspective>(
            50.0f, width / height, 0.1f, 100.0f );

        camera->move( tire::algebra::vector3f{ 0.0f, 0.0f, -10.0f } );
        camera->rotate( 5.0f, 0.0f, 0.0f );

        rndr->addCamera( camera );
    }

    rndr->run();

    return 0;
}
