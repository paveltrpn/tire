
#pragma once

#include <cstddef>
#include <cmath>
#include <array>

#include "concepts.h"

namespace tire::algebra {

template <typename T>
requires Algebraic<T> struct vector3 final {
    using value_type = T;
    using self = vector3<value_type>;

    vector3() {
        data_[0] = value_type{};
        data_[1] = value_type{};
        data_[2] = value_type{};
    }

    vector3( value_type x, value_type y, value_type z ) {
        data_[0] = x;
        data_[1] = y;
        data_[2] = z;
    }

    vector3( const std::array<value_type, 3> &arr ) {
        data_[0] = arr[0];
        data_[1] = arr[1];
        data_[2] = arr[2];
    }

    vector3( const self &rhs ) {
        data_[0] = rhs.data_[0];
        data_[1] = rhs.data_[1];
        data_[2] = rhs.data_[2];
    }

    value_type x() const { return data_[0]; }
    value_type y() const { return data_[1]; }
    value_type z() const { return data_[2]; }

    void plus( const self &b ) {
        for ( size_t i = 0; i < 3; ++i ) data_[i] += b.data_[i];
    };

    void minus( const self &b ) {
        for ( size_t i = 0; i < 3; ++i ) data_[i] -= b.data_[i];
    };

    self inverse() { return { -data_[0], -data_[1], -data_[2] }; }

    template <typename U>
    void scaleSelf( U factor ) {
        data_[0] *= factor;
        data_[1] *= factor;
        data_[2] *= factor;
    }

    template <typename U>
    self scale( U factor ) {
        const auto x = data_[0] * factor;
        const auto y = data_[1] * factor;
        const auto z = data_[2] * factor;
        return { value_type( x ), value_type( y ), value_type( z ) };
    }

    value_type dot( const self &b ) const {
        value_type rt{};

        for ( size_t i = 0; i < 3; ++i ) rt += data_[i] * b.data_[i];

        return rt;
    }

    value_type sqLength() const { return dot( *this ); }

    auto length() -> value_type const {
        // TODO: call proper square root function for float and double
        return std::sqrt( sqLength() );
    }

    void normalizeSelf() {
        value_type len = length();

        if ( len > 0.0 ) {
            for ( size_t i{}; i < 3; ++i ) {
                data_[i] /= len;
            }
        }
    }

    self cross( const self &other ) const {
        auto x = data_[1] * other.data_[2] - data_[2] * other.data_[1];
        auto y = data_[2] * other.data_[0] - data_[0] * other.data_[2];
        auto z = data_[0] * other.data_[1] - data_[1] * other.data_[0];

        return { x, y, z };
    }

    friend self operator+( const self &lhs, const self &rhs ) {
        auto rt = lhs;
        rt.plus( rhs );
        return rt;
    }

    self &operator+=( const self &rhs ) {
        ( *this ).plus( rhs );
        return *this;
    }

    friend self operator-( const self &lhs, const self &rhs ) {
        auto rt = lhs;
        rt.minus( rhs );
        return rt;
    }

    self &operator-=( const self &rhs ) {
        ( *this ).minus( rhs );
        return *this;
    }

    // Internal data pointer getter
    value_type *data() { return data_.data(); }
    const value_type *data() const { return data_.data(); }

    // std::get overload for structured binding vector3 types
    template <size_t I>
    auto &get() & {
        if constexpr ( I == 0 )
            return data_[0];
        else if constexpr ( I == 1 )
            return data_[1];
        else if constexpr ( I == 2 )
            return data_[2];
    }

    template <size_t I>
    auto const &get() const & {
        if constexpr ( I == 0 )
            return data_[0];
        else if constexpr ( I == 1 )
            return data_[1];
        else if constexpr ( I == 2 )
            return data_[2];
    }

    template <size_t I>
    auto &&get() && {
        if constexpr ( I == 0 )
            return std::move( data_[0] );
        else if constexpr ( I == 1 )
            return std::move( data_[1] );
        else if constexpr ( I == 2 )
            return std::move( data_[2] );
    }

private:
    std::array<value_type, 3> data_{};
};

using vector3l = vector3<long long>;
using vector3f = vector3<float>;
using vector3d = vector3<double>;

}  // namespace tire::algebra

// NOTE: namespace std!!!
namespace std {

// Structured binding for vector3f
template <>
struct tuple_size<tire::algebra::vector3f>
    : std::integral_constant<size_t, 3> {};

template <>
struct std::tuple_element<0, tire::algebra::vector3f> {
    using type = float;
};

template <>
struct std::tuple_element<1, tire::algebra::vector3f> {
    using type = float;
};

template <>
struct std::tuple_element<2, tire::algebra::vector3f> {
    using type = float;
};

// Structured binding for vector3d
template <>
struct tuple_size<tire::algebra::vector3d>
    : std::integral_constant<size_t, 3> {};

template <>
struct std::tuple_element<0, tire::algebra::vector3d> {
    using type = double;
};

template <>
struct std::tuple_element<1, tire::algebra::vector3d> {
    using type = double;
};

template <>
struct std::tuple_element<2, tire::algebra::vector3d> {
    using type = double;
};

}  // namespace std
