
module;

#include <cstddef>
#include <cmath>
#include <array>

export module algebra:vector2;

import :concepts;

namespace tire::algebra {

export template <typename T>
    requires Algebraic<T>
struct vector2 final {
    using value_type = T;
    using self = vector2<value_type>;

    vector2() {
        data_[0] = value_type{};
        data_[1] = value_type{};
    }

    vector2( value_type x, value_type y ) {
        data_[0] = x;
        data_[1] = y;
    }

    vector2( const std::array<value_type, 2>& arr ) {
        data_[0] = arr[0];
        data_[1] = arr[1];
    }

    vector2( const self& rhs ) {
        data_[0] = rhs.data_[0];
        data_[1] = rhs.data_[1];
    }

    value_type x() { return data_[0]; }

    value_type y() { return data_[1]; }

    void plus( const self& b ) {
        for ( size_t i = 0; i < 3; ++i ) data_[i] += b.data_[i];
    };

    void minus( const self& b ) {
        for ( size_t i = 0; i < 3; ++i ) data_[i] -= b.data_[i];
    };

    template <typename U>
    void scaleSelf( U factor ) {
        data_[0] *= factor;
        data_[1] *= factor;
    }

    template <typename U>
    self scale( U factor ) {
        const auto x = data_[0] * factor;
        const auto y = data_[1] * factor;
        return { value_type( x ), value_type( y ) };
    }

    value_type dot( const self& b ) const {
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

    friend self operator+( const self& lhs, const self& rhs ) {
        auto rt = lhs;
        rt.plus( rhs );
        return rt;
    }

    self& operator+=( const self& rhs ) {
        ( *this ).plus( rhs );
        return *this;
    }

    friend self operator-( const self& lhs, const self& rhs ) {
        auto rt = lhs;
        rt.minus( rhs );
        return rt;
    }

    self& operator-=( const self& rhs ) {
        ( *this ).minus( rhs );
        return *this;
    }

    value_type* data() { return data_.data(); }

    const value_type* data() const { return data_.data(); }

private:
    std::array<value_type, 2> data_{};
};

export using vector2l = vector2<long long>;
export using vector2f = vector2<float>;
export using vector2d = vector2<double>;

}  // namespace tire::algebra
