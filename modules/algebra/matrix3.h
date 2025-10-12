
#pragma once

#include <cstddef>
#include <initializer_list>
#include <cmath>

#include "concepts.h"
#include "vector3.h"

namespace tire::algebra {

template <typename T>
requires Algebraic<T> struct matrix3 final {
    using value_type = T;
    using self = matrix3<value_type>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;

    matrix3() { idtt(); }

    [[nodiscard]] reference operator[]( size_t index ) { return data_[index]; }

    [[nodiscard]] const_reference operator[]( size_t index ) const {
        return data_[index];
    }

    [[nodiscard]] size_t size() { return data_.size(); }

    [[nodiscard]] pointer data() { return data_.data(); }

    [[nodiscard]] const_pointer data() const { return data_.data(); }

    void zero() {
        data_[0] = T{};
        data_[1] = T{};
        data_[2] = T{};
        data_[3] = T{};
        data_[4] = T{};
        data_[5] = T{};
        data_[6] = T{};
        data_[7] = T{};
        data_[8] = T{};
    }

    void idtt() {
        data_[0] = T{ 1 };
        data_[1] = T{};
        data_[2] = T{};
        data_[3] = T{};
        data_[4] = T{ 1 };
        data_[5] = T{};
        data_[6] = T{};
        data_[7] = T{};
        data_[8] = T{ 1 };
    }

    matrix3( std::initializer_list<value_type> list ) {
        for ( auto i = 0; const auto e : list ) {
            data_[i] = e;
            ++i;
        }
    }

    [[nodiscard]] self transpose() {
        value_type tmp;
        auto rt = *this;

        for ( size_t i = 0; i < 3; ++i ) {
            for ( size_t j = 0; j < i; ++j ) {
                tmp = rt[j, i];
                rt[j, i] = rt[i, j];
                rt[i, j] = tmp;
            }
        }

        return rt;
    }

    void transposeSelf() {
        value_type tmp;
        for ( size_t i = 0; i < 3; ++i ) {
            for ( size_t j = 0; j < i; ++j ) {
                tmp = ( *this )[j, i];
                ( *this )[j, i] = ( *this )[i, j];
                ( *this )[i, j] = tmp;
            }
        }
    }

    void multiply( const self &rhs ) {
        /*auto this00 = ( *this )[0, 0];
        auto this01 = ( *this )[0, 1];

        ( *this )[0, 0] = this00 * rhs[0, 0] + this01 * rhs[1, 0] +
                          ( *this )[0, 2] * rhs[2, 0];
        ( *this )[0, 1] = this00 * rhs[0, 1] + this01 * rhs[1, 1] +
                          ( *this )[0, 2] * rhs[2, 1];
        ( *this )[0, 2] = this00 * rhs[0, 2] + this01 * rhs[1, 2] +
                          ( *this )[0, 2] * rhs[2, 2];

        auto this10 = ( *this )[1, 0];
        auto this11 = ( *this )[1, 1];

        ( *this )[1, 0] = this10 * rhs[0, 0] + this11 * rhs[1, 0] +
                          ( *this )[1, 2] * rhs[2, 0];
        ( *this )[1, 1] = this10 * rhs[0, 1] + this11 * rhs[1, 1] +
                          ( *this )[1, 2] * rhs[2, 1];
        ( *this )[1, 2] = this10 * rhs[0, 2] + this11 * rhs[1, 2] +
                          ( *this )[1, 2] * rhs[2, 2];

        auto this20 = ( *this )[2, 0];
        auto this21 = ( *this )[2, 1];

        ( *this )[2, 0] = this20 * rhs[0, 0] + this21 * rhs[1, 0] +
                          ( *this )[2, 2] * rhs[2, 0];
        ( *this )[2, 1] = this20 * rhs[0, 1] + this21 * rhs[1, 1] +
                          ( *this )[2, 2] * rhs[2, 1];
        ( *this )[2, 2] = this20 * rhs[0, 2] + this21 * rhs[1, 2] +
                          ( *this )[2, 2] * rhs[2, 2];*/
    }

    self operator*( const self &rhs ) {
        auto tmp{ *this };
        tmp.multiply( rhs );
        *this = tmp;
        return *this;
    }

    vector3<value_type> mult_vector3( const vector3<value_type> &v ) {
        vector3<value_type> rt;
        value_type w;

        rt[0] = v[0] * ( *this )[0] + v[1] * ( *this )[1] + v[2] * ( *this )[2];
        rt[1] = v[0] * ( *this )[3] + v[1] * ( *this )[4] + v[2] * ( *this )[5];
        rt[2] = v[0] * ( *this )[6] + v[1] * ( *this )[6] + v[2] * ( *this )[8];

        return rt;
    }

    value_type determinant() {}

    void rotation( value_type yaw, value_type pitch, value_type roll ) {
        auto siny = std::sin( yaw );
        auto cosy = std::cos( yaw );
        auto sinp = std::sin( pitch );
        auto cosp = std::cos( pitch );
        auto sinr = std::sin( roll );
        auto cosr = std::cos( roll );

        data_[0] = cosy * cosr - siny * cosp * sinr;
        data_[1] = -cosy * sinr - siny * cosp * cosr;
        data_[2] = siny * sinp;

        data_[3] = siny * cosr + cosy * cosp * sinr;
        data_[4] = -siny * sinr + cosy * cosp * cosr;
        data_[5] = -cosy * sinp;

        data_[6] = sinp * sinr;
        data_[7] = sinp * cosr;
        data_[8] = cosp;
    }

private:
    std::array<value_type, 9> data_;
};

using matrix3l = matrix3<long long>;
using matrix3f = matrix3<float>;
using matrix3d = matrix3<double>;

}  // namespace tire::algebra
