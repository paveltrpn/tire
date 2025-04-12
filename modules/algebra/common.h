
#pragma once

#include <cmath>
#include <concepts>
#include <numbers>
#include <type_traits>

#include "algebra/concepts.h"

namespace tire::algebra {

template <std::floating_point T>
constexpr T _half_circle = T{ 180 };

template <std::floating_point T>
constexpr T degToRad( T deg ) {
    return deg * ( std::numbers::pi_v<T> / _half_circle<T> );
}

template <std::floating_point T>
constexpr T radToDeg( T rad ) {
    return rad * ( _half_circle<T> / std::numbers::pi_v<T> );
}

}  // namespace tire::algebra