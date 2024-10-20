
#pragma once

#include <cstddef>
#include <cmath>
#include <array>

namespace tire::algebra
{

template <typename T, size_t size> struct vector_base {
    using self = vector_base<T, size>;
    using value_type = T;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;

    vector_base() = default;

    vector_base( std::initializer_list<value_type> list ) {
        for ( auto i = 0; const auto e : list ) {
            data_[i] = e;
            ++i;
        }
    }

    [[nodiscard]] reference operator[]( size_t index ) { return data_[index]; }

    [[nodiscard]] const_reference operator[]( size_t index ) const { return data_[index]; }

    void plus( const self &b ) {
        for ( size_t i = 0; i < size; ++i )
            data_[i] += b.data_[i];
    };

    void minus( const self &b ) {
        for ( size_t i = 0; i < size; ++i )
            data_[i] -= b.data_[i];
    };

    template <typename U> void scale( U factor ) {
        for ( size_t i = 0; i < size; ++i )
            data_[i] *= factor;
    }

    value_type dot( const self &b ) const {
        value_type rt{};

        for ( size_t i = 0; i < size; ++i )
            rt += data_[i] * b.data_[i];

        return rt;
    }

    value_type sqLenght() const { return dot( *this ); }

    auto lenght() -> value_type const { return std::sqrt( sqLenght() ); }

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

    pointer data() { return data_.data(); }

    const_pointer data() const { return data_.data(); }

protected:
    std::array<value_type, size> data_;
};

} // namespace tire::algebra
