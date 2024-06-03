
#ifndef __tire_point_h__
#define __tire_point_h__

#include "algebra/Vector.h"
#include "algebra/Matrix.h"

namespace tire {

template <typename T = float>
struct point3 {
        using scalar_type = T;
        using self = point3<scalar_type>;
        using vec_type = tire::algebra::Vector3<scalar_type>;
        using mat3_type = tire::algebra::Matrix<scalar_type, 3>;
        using mat4_type = tire::algebra::Matrix<scalar_type, 4>;

        point3() : pos_{} {
        }

        point3(scalar_type x, scalar_type y, scalar_type z) : pos_{ x, y, z } {
        }

        point3(vec_type rhs) : pos_{ rhs } {
        }

        scalar_type x() {
            return pos_.x();
        }

        scalar_type y() {
            return pos_.y();
        }

        scalar_type z() {
            return pos_.z();
        }

        void move(vec_type offst) {
            pos_.sumSelf(offst);
        }

        void transform(mat3_type mtrx) {
            pos_ = mtrx.mult(pos_);
        }

    private:
        vec_type pos_;
};

using point3i = point3<long long>;
using point3f = point3<float>;
using point3d = point3<double>;

}  // namespace tire

#endif
