
#pragma once

#include "algebra/vector3.h"
#include "algebra/matrix3.h"
#include "algebra/matrix4.h"

namespace tire {

template <typename T>
struct point3 {
    using scalar_type = T;
    using self = point3<scalar_type>;
    using vec_type = algebra::vector3<scalar_type>;
    using mat3_type = algebra::matrix3<scalar_type>;
    using mat4_type = algebra::matrix4<scalar_type>;

    point3() = default;

    point3( scalar_type x, scalar_type y, scalar_type z )
        : pos_{ x, y, z } {}

    point3( vec_type rhs )
        : pos_{ rhs } {}

    [[nodiscard]] scalar_type x() const { return pos_.x(); }
    [[nodiscard]] scalar_type y() const { return pos_.y(); }
    [[nodiscard]] scalar_type z() const { return pos_.z(); }

    [[nodiscard]] self move( vec_type offst ) const {
        auto pos = pos_;
        pos.plus( offst );
        return pos;
    }
    void moveThis( vec_type offst ) { pos_.plus( offst ); }
    [[nodiscard]] self transform( mat3_type mtrx ) const {
        return mtrx.mult_vector3( pos_ );
    }
    void transformThis( mat3_type mtrx ) { pos_ = mtrx.mult_vector3( pos_ ); }
    [[nodiscard]] self transform( mat4_type mtrx ) const {
        return mtrx.mult_vector3( pos_ );
    }
    void transformThis( mat4_type mtrx ) { pos_ = mtrx.mult_vector3( pos_ ); }

private:
    vec_type pos_{};
};

using point3l = point3<long long>;
using point3f = point3<float>;
using point3d = point3<double>;

}  // namespace tire
