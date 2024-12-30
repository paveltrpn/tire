
#include "color.h"

namespace tire {

Color::Color( const std::string& str ) {
    if ( str.starts_with( "#" ) ) {
        const auto c = charFromHex( str );
        r_ = c[0] / 255.0;
        g_ = c[1] / 255.0;
        b_ = c[2] / 255.0;
        a_ = c[3] / 255.0;
    } else {
    }
}

std::array<unsigned char, 4> Color::charFromHex( const std::string& letters ) {
    std::array<unsigned char, 4> ret;
    size_t l{};
    for ( size_t i = 1; i < letters.length(); i += 2 ) {
        const auto byteString = std::string{ letters.substr( i, 2 ) };
        const auto byteValue =
            static_cast<unsigned char>( std::stoi( byteString, nullptr, 16 ) );
        ret[l] = byteValue;
        ++l;
    }
    return ret;
}

}  // namespace tire
