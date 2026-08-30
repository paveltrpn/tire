
#include <sstream>
#include <cstring>

#include "log.h"

namespace tire::log {

auto elideRight( const char *str, int after ) -> std::string {
    const auto length = std::strlen( str );
    auto elided = std::stringstream{};

    if ( length > after ) {
        elided << std::format( "{}...", std::string_view( str ).substr( 0, length - after ) );
    } else {
        elided << str;
    }

    return elided.str();
}

auto elideLeft( const char *str, int after ) -> std::string {
    const auto length = std::strlen( str );
    auto elided = std::stringstream{};

    if ( length > after ) {
        elided << std::format( "...{}", std::string_view( str ).substr( length - after, length ) );
    } else {
        elided << str;
    }

    return elided.str();
}

}  // namespace tire::log
