
#pragma once

#include <limits>

import toy_std;

namespace tire {

    template <typename T = float>
    struct normal {
            using scalar_type = T;
            using self = normal<scalar_type>;
            using vec_type = toy::algebra::vector3<scalar_type>;
            using mat3_type = toy::algebra::matrix3<scalar_type>;
            using mat4_type = toy::algebra::matrix4<scalar_type>;

            scalar_type x() {
                return normal_.x();
            }

            scalar_type y() {
                return normal_.y();
            }

            scalar_type z() {
                return normal_.z();
            }

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

            // void transform(mat3_type rhs) {
            //     auto i = rhs.inverse();
            //     auto ti = i.transpose();
            //     normal_ = ti.mult(normal_);
            // }

        private:
            vec_type normal_;
    };

    using normall = normal<long long>;
    using normald = normal<double>;
}  // namespace tire
