
#include <iostream>
#include <stdexcept>

#include "log/log.h"

#include "geometry/wavefront.h"

int main( int argc, char **argv ) {
    try {
        tire::WavefrontObj demon{ "/mnt/main_disk/code/tiny_render/assets/demon_baby.obj" };
        std::cout << demon;
    } catch ( const std::runtime_error &e ) {
        tire::log::error( "catch exception: {}", e.what() );
        return 0;
    }

    return 0;
}
