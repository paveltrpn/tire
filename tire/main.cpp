
#include "log/log.h"
#include "window.h"

auto main( int argc, char **argv ) -> int {
    tire::log::info()( "start r..." );
    try {
        auto foo = BareWindow{};
        foo.loop();
    } catch ( const std::exception &e ) {
        tire::log::error()( "caught exception: {}", e.what() );
        return 1;
    }
    return 0;
}
