
#include <format>

#include "mesh.h"

#include "config/config.h"

namespace tire {

Mesh::Mesh( const std::string& path ) {
    std::ifstream file{ path };
    if ( !file ) {
        throw std::runtime_error(
            std::format( "Mesh === file not found: {}\n", path ) );
    }

    file.close();
}

}  // namespace tire