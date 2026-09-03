
#include "log/log.h"

import barewindow;

auto main( int argc, char** argv ) -> int {
    tire::log::info()( "start tire..." );
    try {
        auto foo = BareWindow{};
        foo.loop();
    } catch ( const std::exception& e ) {
        tire::log::error()( "caught exception: {}", e.what() );
        return 1;
    }
    return 0;
}
