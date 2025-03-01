
#pragma once

#include "algebra/concepts.h"
#include "vector.h"

namespace tire::algebra {

template <typename T>
requires Algebraic<T> struct vector4 : vector_base<T, 4> {
    using base_type = vector_base<T, 4>;
    using base_type::data_;
    using typename base_type::self;
    using typename base_type::value_type;

    vector4() {
        data_[0] = T{};
        data_[1] = T{};
        data_[2] = T{};
        data_[3] = T{};
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

    vector4( const self &rhs ) {
        ( *this )[0] = rhs[0];
        ( *this )[1] = rhs[1];
        ( *this )[2] = rhs[2];
        ( *this )[3] = rhs[3];
    }

    value_type x() { return data_[0]; }
    value_type y() { return data_[1]; }
    value_type z() { return data_[2]; }
    value_type w() { return data_[3]; }
};

using vector4l = vector4<long long>;
using vector4f = vector4<float>;
using vector4d = vector4<double>;

}  // namespace tire::algebra
