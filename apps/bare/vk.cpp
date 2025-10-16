
#include <memory>
#include <filesystem>

#include "log/log.h"
#include "render/rendervk.h"
#include "bare.h"

#include "config/config.h"

auto main( int argc, char** argv ) -> int {
    try {
        new tire::Config{ "apps/config.json" };
        auto foo = BareWindow{};
        foo.loop();
    } catch ( const std::exception& e ) {
        tire::log::error( "caught exception: {}", e.what() );
        return 1;
    }
    return 0;
}
