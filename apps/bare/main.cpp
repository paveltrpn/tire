
#include "window.h"
#include "config/config.h"

import log;

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
