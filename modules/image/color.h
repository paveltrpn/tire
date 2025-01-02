
#pragma once

#include <string>
#include <array>
#include <map>
#include <concepts>
#include <type_traits>

#include "log/log.h"

namespace tire {

namespace __details {

static const std::map<std::string, std::string> htmlColors{
    { "powderblue", "#B0E0E6" },
    { "lightblue", "#ADD8E6" },
    { "lightskyblue", "#87CEFA" },
    { "skyblue", "#87CEEB" },
    { "deepskyblue", "#00BFFF" },
    { "lightsteelblue", "#B0C4DE" },
    { "dodgerblue", "#1E90FF" },
    { "cornflowerblue", "#6495ED" },
    { "steelblue", "#4682B4" },
    { "royalblue", "#4169E1" },
    { "blue", "#0000FF" },
    { "mediumblue", "#0000CD" },
    { "darkblue", "#00008B" },
    { "navy", "#000080" },
    { "midnightblue", "#191970" },
    { "mediumslateblue", "#7B68EE" },
    { "slateblue", "#6A5ACD" },
    { "darkslateblue", "#483D8B" },
    { "lawngreen", "#7CFC00" },
    { "chartreuse", "#7FFF00" },
    { "limegreen", "#32CD32" },
    { "lime", "#00FF00" },
    { "forestgreen", "#228B22" },
    { "green", "#008000" },
    { "darkgreen", "#006400" },
    { "greenyellow", "#ADFF2F" },
    { "yellowgreen", "#9ACD32" },
    { "springgreen", "#00FF7F" },
    { "mediumspringgreen", "#00FA9A" },
    { "lightgreen", "#90EE90" },
    { "palegreen", "#98FB98" },
    { "darkseagreen", "#8FBC8F" },
    { "mediumseagreen", "#3CB371" },
    { "seagreen", "#2E8B57" },
    { "olive", "#808000" },
    { "darkolivegreen", "#556B2F" },
    { "olivedrab", "#6B8E23" },
    { "lightsalmon", "#FFA07A" },
    { "salmon", "#FA8072" },
    { "darksalmon", "#E9967A" },
    { "lightcoral", "#F08080" },
    { "indianred", "#CD5C5C" },
    { "crimson", "#DC143C" },
    { "firebrick", "#B22222" },
    { "red", "#FF0000" },
    { "darkred", "#8B0000" },
    { "gainsboro", "#DCDCDC" },
    { "lightgray", "#D3D3D3" },
    { "silver", "#C0C0C0" },
    { "darkgray", "#A9A9A9" },
    { "gray", "#808080" },
    { "dimgray", "#696969" },
    { "lightslategray", "#778899" },
    { "slategray", "#708090" },
    { "darkslategray", "#2F4F4F" },
    { "black", "#000000" },
};

}

template <typename T>
struct Rgba {
    T r_{};
    T g_{};
    T b_{};
    T a_{};
};

template <typename T>
concept ColorValue = (std::unsigned_integral<T> &&
                      std::is_same_v<T, uint8_t>) ||
                     ( std::floating_point<T> && std::is_same_v<T, float> );

template <ColorValue T>
struct Color {
    using value_type = T;
    using char_type = uint8_t;

    Color() = default;
    Color( value_type a, value_type r, value_type g, value_type b )
        : a_{ a }
        , r_{ r }
        , g_{ g }
        , b_{ b } {};

    virtual ~Color() = default;

    value_type r() const { return r_; };
    value_type g() const { return g_; };
    value_type b() const { return b_; };
    value_type a() const { return a_; };

protected:
    std::array<char_type, 4> charFromHex( std::string_view letters ) {
        std::array<char_type, 4> ret{ 255, 255, 255, 255 };
        const auto length = letters.length();
        size_t l{};

        if ( ( length == 1 ) || ( length < 7 ) || ( ( length % 2 ) == 0 ) ||
             ( length > 9 ) ) {
            log::warning( "broken color hex string!" );
            return { 255, 255, 255, 255 };
        }

        for ( size_t i = 1; i < letters.length(); i += 2 ) {
            const auto byteString = letters.substr( i, 2 );
            const auto byteValue = static_cast<char_type>(
                std::stoi( std::string{ byteString }, nullptr, 16 ) );
            ret[l] = byteValue;
            ++l;
        }
        return ret;
    };

protected:
    value_type r_{};
    value_type g_{};
    value_type b_{};
    value_type a_{};
};

struct Colorf final : Color<float> {
    using value_type = Color<float>::value_type;
    using char_type = Color<float>::char_type;

    Colorf()
        : Color{} {}

    Colorf( const std::string& str )
        : Color{} {
        if ( str.starts_with( "#" ) ) {
            const auto [r, g, b, a] = charFromHex( str );
            r_ = r / 255.0f;
            g_ = g / 255.0f;
            b_ = b / 255.0f;
            a_ = a / 255.0f;
        } else {
            try {
                const auto& hex = __details::htmlColors.at( str );
                const auto [r, g, b, a] = charFromHex( hex );
                r_ = r / 255.0f;
                g_ = g / 255.0f;
                b_ = b / 255.0f;
                a_ = a / 255.0f;
            } catch ( std::out_of_range& e ) {
                log::warning( "there is no color {}", str );
                r_ = g_ = b_ = a_ = 1.0f;
            }
        }
    };
};

struct Colori final : Color<uint8_t> {
    using value_type = Color<uint8_t>::value_type;
    using char_type = Color<uint8_t>::char_type;

    Colori()
        : Color{} {}

    Colori( const std::string& str )
        : Color{} {
        if ( str.starts_with( "#" ) ) {
            const auto [r, g, b, a] = charFromHex( str );
            r_ = r;
            g_ = g;
            b_ = b;
            a_ = a;
        } else {
            try {
                const auto& hex = __details::htmlColors.at( str );
                const auto [r, g, b, a] = charFromHex( hex );
                r_ = r;
                g_ = g;
                b_ = b;
                a_ = a;
            } catch ( std::out_of_range& e ) {
                log::warning( "there is no color {}", str );
                r_ = g_ = b_ = a_ = 255;
            }
        }
    };
};

}  // namespace tire
