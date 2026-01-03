
module;

#include <cstddef>
#include <cmath>
#include <array>

export module algebra:vector4;

import :concepts;
import :vector3;

namespace tire::algebra {

export template <typename T>
    requires Algebraic<T>
struct vector4 final {
    using value_type = T;
    using self = vector4<value_type>;

    vector4() {
        data_[0] = value_type{};
        data_[1] = value_type{};
        data_[2] = value_type{};
        data_[3] = value_type{};
    }

    vector4( value_type x, value_type y, value_type z, value_type w ) {
        data_[0] = x;
        data_[1] = y;
        data_[2] = z;
        data_[3] = w;
    }

    vector4( const std::array<value_type, 4> &arr ) {
        data_[0] = arr[0];
        data_[1] = arr[1];
        data_[2] = arr[2];
        data_[3] = arr[3];
    }

    vector4( const std::array<value_type, 3> &arr, value_type w = 0.0 ) {
        data_[0] = arr[0];
        data_[1] = arr[1];
        data_[2] = arr[2];
        data_[3] = w;
    }

    vector4( const vector3<value_type> other, value_type w = 0.0 ) {
        data_[0] = other.x();
        data_[1] = other.y();
        data_[2] = other.z();
        data_[3] = w;
    }

    vector4( const self &rhs ) {
        data_[0] = rhs.data_[0];
        data_[1] = rhs.data_[1];
        data_[2] = rhs.data_[2];
        data_[3] = rhs.data_[3];
    }

    vector4( vector4 &&other ) = default;

    auto operator=( const vector4 &other ) -> vector4 & = default;
    auto operator=( vector4 &&other ) -> vector4 & = default;

    ~vector4() = default;

    auto x() const -> value_type {
        //
        return data_[0];
    }

    auto y() const -> value_type {
        // /
        return data_[1];
    }

    auto z() const -> value_type {
        //
        return data_[2];
    }

    auto w() const -> value_type {
        //
        return data_[3];
    }

    auto plus( const self &b ) -> void {
        for ( size_t i = 0; i < 3; ++i ) data_[i] += b.data_[i];
    };

    auto minus( const self &b ) -> void {
        for ( size_t i = 0; i < 3; ++i ) data_[i] -= b.data_[i];
    };

    template <typename U>
    void scaleSelf( U factor ) {
        data_[0] *= factor;
        data_[1] *= factor;
        data_[2] *= factor;
        data_[3] *= factor;
    }

    template <typename U>
    auto scale( U factor ) -> self {
        const auto x = data_[0] * factor;
        const auto y = data_[1] * factor;
        const auto z = data_[2] * factor;
        const auto w = data_[3] * factor;
        return { value_type( x ), value_type( y ), value_type( z ), value_type( w ) };
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

    value_type *data() {
        //
        return data_.data();
    }

    const value_type *data() const {
        //
        return data_.data();
    }

    // std::get overload for structured binding vector4 types
    template <size_t I>
    auto get() -> value_type & {
        if constexpr ( I == 0 )
            return data_[0];
        else if constexpr ( I == 1 )
            return data_[1];
        else if constexpr ( I == 2 )
            return data_[2];
        else if constexpr ( I == 3 )
            return data_[3];
    }

    template <size_t I>
    auto get() const -> value_type const & {
        if constexpr ( I == 0 )
            return data_[0];
        else if constexpr ( I == 1 )
            return data_[1];
        else if constexpr ( I == 2 )
            return data_[2];
        else if constexpr ( I == 3 )
            return data_[3];
    }

    template <size_t I>
    auto get() -> value_type && {
        if constexpr ( I == 0 )
            return std::move( data_[0] );
        else if constexpr ( I == 1 )
            return std::move( data_[1] );
        else if constexpr ( I == 2 )
            return std::move( data_[2] );
        else if constexpr ( I == 3 )
            return std::move( data_[3] );
        ;
    }

private:
    std::array<value_type, 4> data_{};
};

export using vector4l = vector4<long long>;
export using vector4f = vector4<float>;
export using vector4d = vector4<double>;

}  // namespace tire::algebra

// NOTE: namespace std!!!
namespace std {

// Structured binding for vector3f
template <>
struct tuple_size<tire::algebra::vector4f> : std::integral_constant<size_t, 4> {};

template <>
struct tuple_element<0, tire::algebra::vector4f> {
    using type = float;
};

template <>
struct tuple_element<1, tire::algebra::vector4f> {
    using type = float;
};

template <>
struct tuple_element<2, tire::algebra::vector4f> {
    using type = float;
};

template <>
struct tuple_element<3, tire::algebra::vector4f> {
    using type = float;
};

// Structured binding for vector3d
template <>
struct tuple_size<tire::algebra::vector4d> : std::integral_constant<size_t, 4> {};

template <>
struct tuple_element<0, tire::algebra::vector4d> {
    using type = double;
};

template <>
struct tuple_element<1, tire::algebra::vector4d> {
    using type = double;
};

template <>
struct tuple_element<2, tire::algebra::vector4d> {
    using type = double;
};

template <>
struct tuple_element<3, tire::algebra::vector4d> {
    using type = double;
};

}  // namespace std
