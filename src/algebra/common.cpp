
module;

#include <cmath>
#include <concepts>
#include <numbers>
#include <type_traits>

export module algebra:common;

import :concepts;

namespace tire::algebra {

export template <std::floating_point T>
constexpr T _half_circle = T{ 180 };

export template <std::floating_point T>
constexpr T degToRad( T deg ) {
    return deg * ( std::numbers::pi_v<T> / _half_circle<T> );
}

export template <std::floating_point T>
constexpr T radToDeg( T rad ) {
    return rad * ( _half_circle<T> / std::numbers::pi_v<T> );
}

}  // namespace tire::algebra
