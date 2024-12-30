
#include <stdexcept>

#include "log/log.h"

#include "image/color.h"

int main( int argc, char **argv ) {
    try {
        std::string a = "#1cff0000";
        tire::Color demon{ a };
        tire::log::debug<true>( "r = {}; g = {}; b = {}; a = {}", demon.r(),
                                demon.g(), demon.b(), demon.a() );
    } catch ( const std::runtime_error &e ) {
        tire::log::error( "catch exception: {}", e.what() );
        return 0;
    }

    return 0;
}
