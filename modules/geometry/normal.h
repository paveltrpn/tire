
#pragma once

#include <limits>

#include "algebra/vector3.h"
#include "algebra/matrix3.h"
#include "algebra/matrix4.h"

namespace tire {

template <typename T>
struct normal {
    using scalar_type = T;
    using self = normal<scalar_type>;
    using vec_type = algebra::vector3<scalar_type>;
    using mat3_type = algebra::matrix3<scalar_type>;
    using mat4_type = algebra::matrix4<scalar_type>;

    normal() = default;

    normal( scalar_type x, scalar_type y, scalar_type z )
        : normal_{ x, y, z } {}

    normal( vec_type rhs )
        : normal_{ rhs } {}

    [[nodiscard]] scalar_type x() const { return normal_[0]; }
    [[nodiscard]] scalar_type y() const { return normal_[1]; }
    [[nodiscard]] scalar_type z() const { return normal_[2]; }

    // scalar_type dot(vec_type rhs) {
    //     return normal_.dot(rhs);
    // }

    // sualar_type squareLength() {
    //     return normal_.dot(normal_);
    // }

    // scalar_type lenght() {
    //     return normal_.length();
    // }

    // void normalize() {
    //     normal_.normalize();
    // }

    [[nodiscard]] self transform( mat3_type mtrx ) const {
        auto i = mtrx.inverse();
        i.transposeSelf();
        return i.mult_vector3( normal_ );
    }

    void transformThis( mat3_type mtrx ) {
        auto i = mtrx.inverse();
        i.transposeSelf();
        normal_ = i.mult_vector3( normal_ );
    }

    [[nodiscard]] self transform( mat4_type mtrx ) const {
        // Transorm normal by inversed and transposed transformation matrix.
        auto i = mtrx.inverse();
        i.transposeSelf();
        return mtrx.mult_vector3( normal_ );
    }

    void transformThis( mat4_type mtrx ) {
        auto i = mtrx.inverse();
        i.transposeSelf();
        normal_ = i.mult_vector3( normal_ );
    }

private:
    vec_type normal_{};
};

using normall = normal<long long>;
using normalf = normal<float>;
using normald = normal<double>;

}  // namespace tire
