
#include <iostream>
#include <stdexcept>

#include "log/log.h"

import wavefront;

int main( int argc, char **argv ) {
    try {
        tire::Obj demon{ "/mnt/main_disk/code/tiny_render/assets/demon_baby.obj" };
        std::cout << demon;
    } catch ( const std::runtime_error &e ) {
        tire::log::error( "catch exception: {}", e.what() );
        return 0;
    }

    return 0;
}
