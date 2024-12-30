
#pragma once

#include <string>
#include <array>

namespace tire {

struct Color final {
    Color( float a, float r, float g, float b )
        : a_{ a }
        , r_{ r }
        , g_{ g }
        , b_{ b } {};
    Color( const std::string& str );

    float r() { return r_; };
    float g() { return g_; };
    float b() { return b_; };
    float a() { return a_; };

private:
    std::array<unsigned char, 4> charFromHex( const std::string& letters );

private:
    float r_{};
    float g_{};
    float b_{};
    float a_{};
};

}  // namespace tire
