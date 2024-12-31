

#pragma once

#include "vector.h"

namespace tire::algebra {

template <typename T>
struct vector3 : vector_base<T, 3> {
    using base_type = vector_base<T, 3>;
    using base_type::data_;
    using typename base_type::self;
    using typename base_type::value_type;

    vector3() {
        data_[0] = T{};
        data_[1] = T{};
        data_[2] = T{};
    }

    vector3( value_type x, value_type y, value_type z ) {
        data_[0] = x;
        data_[1] = y;
        data_[2] = z;
    }

    vector3( const self &rhs ) {
        ( *this )[0] = rhs[0];
        ( *this )[1] = rhs[1];
        ( *this )[2] = rhs[2];
    }

    value_type x() { return data_[0]; }
    value_type y() { return data_[1]; }
    value_type z() { return data_[2]; }
};

using vector3l = vector3<long long>;
using vector3f = vector3<float>;
using vector3d = vector3<double>;

}  // namespace tire::algebra
