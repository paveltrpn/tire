
module;

#include <cstddef>
#include <cmath>
#include <array>

export module algebra:vector3;

import :concepts;

namespace tire::algebra {

export template <typename T>
    requires Algebraic<T>
struct vector3 final {
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

    auto x() const -> value_type {
        //
        return data_[0];
    }

    auto y() const -> value_type {
        //
        return data_[1];
    }

    auto z() const -> value_type {
        //
        return data_[2];
    }

    auto plus( const self &b ) -> void {
        for ( size_t i = 0; i < 3; ++i ) data_[i] += b.data_[i];
    };

    auto minus( const self &b ) -> void {
        for ( size_t i = 0; i < 3; ++i ) data_[i] -= b.data_[i];
    };

    auto inverse() -> self {
        //
        return { -data_[0], -data_[1], -data_[2] };
    }

    template <typename U>
    auto scaleSelf( U factor ) -> void {
        data_[0] *= factor;
        data_[1] *= factor;
        data_[2] *= factor;
    }

    template <typename U>
    auto scale( U factor ) -> self {
        const auto x = data_[0] * factor;
        const auto y = data_[1] * factor;
        const auto z = data_[2] * factor;
        return { value_type( x ), value_type( y ), value_type( z ) };
    }

    auto dot( const self &b ) const -> value_type {
        value_type rt{};

        for ( size_t i = 0; i < 3; ++i ) rt += data_[i] * b.data_[i];

        return rt;
    }

    auto sqLength() const -> value_type {
        //
        return dot( *this );
    }

    auto length() -> value_type const {
        // TODO: call proper square root function for float and double
        return std::sqrt( sqLength() );
    }

    auto normalizeSelf() -> void {
        value_type len = length();

        if ( len > 0.0 ) {
            for ( size_t i{}; i < 3; ++i ) {
                data_[i] /= len;
            }
        }
    }

    auto cross( const self &other ) const -> self {
        auto x = data_[1] * other.data_[2] - data_[2] * other.data_[1];
        auto y = data_[2] * other.data_[0] - data_[0] * other.data_[2];
        auto z = data_[0] * other.data_[1] - data_[1] * other.data_[0];

        return { x, y, z };
    }

    auto operator[]( size_t index ) -> value_type & {
        //
        return data_[index];
    }

    auto operator[]( size_t index ) const -> const value_type & {
        //
        return data_[index];
    }

    friend auto operator+( const self &lhs, const self &rhs ) -> self {
        auto rt = lhs;
        rt.plus( rhs );
        return rt;
    }

    auto operator+=( const self &rhs ) -> self & {
        ( *this ).plus( rhs );
        return *this;
    }

    friend auto operator-( const self &lhs, const self &rhs ) -> self {
        auto rt = lhs;
        rt.minus( rhs );
        return rt;
    }

    auto operator-=( const self &rhs ) -> self & {
        ( *this ).minus( rhs );
        return *this;
    }

    // Internal data pointer getter
    auto data() -> value_type * {
        //
        return data_.data();
    }

    auto data() const -> const value_type * {
        //
        return data_.data();
    }

    // std::get overload for structured binding vector3 types
    template <size_t I>
    auto get() -> value_type & {
        if constexpr ( I == 0 )
            return data_[0];
        else if constexpr ( I == 1 )
            return data_[1];
        else if constexpr ( I == 2 )
            return data_[2];
    }

    template <size_t I>
    auto get() const -> value_type const & {
        if constexpr ( I == 0 )
            return data_[0];
        else if constexpr ( I == 1 )
            return data_[1];
        else if constexpr ( I == 2 )
            return data_[2];
    }

    template <size_t I>
    auto get() -> value_type && {
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

export using vector3l = vector3<long long>;
export using vector3f = vector3<float>;
export using vector3d = vector3<double>;

}  // namespace tire::algebra

// NOTE: namespace std!!!
namespace std {

// Structured binding for vector3f
template <>
struct tuple_size<tire::algebra::vector3f> : std::integral_constant<size_t, 3> {};

template <>
struct tuple_element<0, tire::algebra::vector3f> {
    using type = float;
};

template <>
struct tuple_element<1, tire::algebra::vector3f> {
    using type = float;
};

template <>
struct tuple_element<2, tire::algebra::vector3f> {
    using type = float;
};

// Structured binding for vector3d
template <>
struct tuple_size<tire::algebra::vector3d> : std::integral_constant<size_t, 3> {};

template <>
struct tuple_element<0, tire::algebra::vector3d> {
    using type = double;
};

template <>
struct tuple_element<1, tire::algebra::vector3d> {
    using type = double;
};

template <>
struct tuple_element<2, tire::algebra::vector3d> {
    using type = double;
};

}  // namespace std
