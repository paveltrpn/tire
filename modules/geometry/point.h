
#pragma once

#include "algebra/vector3.h"
#include "algebra/matrix3.h"

namespace tire {
template <typename T>
struct point3 {
    using scalar_type = T;
    using self = point3<scalar_type>;
    using vec_type = algebra::vector3<scalar_type>;
    using mat3_type = algebra::matrix3<scalar_type>;
    using mat4_type = algebra::matrix3<scalar_type>;

    point3()
        : pos_{} {}

    point3( scalar_type x, scalar_type y, scalar_type z )
        : pos_{ x, y, z } {}

    point3( vec_type rhs )
        : pos_{ rhs } {}

    scalar_type x() { return pos_.x(); }
    scalar_type y() { return pos_.y(); }
    scalar_type z() { return pos_.z(); }

    void move( vec_type offst ) { pos_.plus( offst ); }
    void transform( mat3_type mtrx ) { pos_ = mtrx.mult_vector3( pos_ ); }

private:
    vec_type pos_;
};

using point3l = point3<long long>;
using point3f = point3<float>;
using point3d = point3<double>;
}  // namespace tire
