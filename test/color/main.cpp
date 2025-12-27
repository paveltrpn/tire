
#include <stdexcept>

import log;
import image;

int main( int argc, char** argv ) {
    try {
        tire::Colori silver{ "silver" };
        tire::log::debug<true>( "silver r = {}; g = {}; b = {}; a = {}",
                                silver.r(), silver.g(), silver.b(),
                                silver.a() );

        tire::Colori alpharedi{ "#ff00001c" };
        tire::log::debug<true>( "alpharedi r = {}; g = {}; b = {}; a = {}",
                                alpharedi.r(), alpharedi.g(), alpharedi.b(),
                                alpharedi.a() );

        tire::Colorf alpharedf{ "#ff00001c" };
        tire::log::debug<true>( "alpharedf r = {}; g = {}; b = {}; a = {}",
                                alpharedf.r(), alpharedf.g(), alpharedf.b(),
                                alpharedf.a() );

        tire::Colorf gainsborof{ "gainsboro" };
        tire::log::debug<true>( "gainsborof r = {}; g = {}; b = {}; a = {}",
                                gainsborof.r(), gainsborof.g(), gainsborof.b(),
                                gainsborof.a() );

        tire::Colori deepskyblue{ "deepskyblue" };
        tire::log::debug<true>( "deepskybluei r = {}; g = {}; b = {}; a = {}",
                                deepskyblue.r(), deepskyblue.g(),
                                deepskyblue.b(), deepskyblue.a() );
    } catch ( const std::runtime_error& e ) {
        tire::log::error( "catch exception: {}", e.what() );
        return 0;
    }

    return 0;
}
