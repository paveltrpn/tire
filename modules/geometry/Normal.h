
#ifndef __tire_normal_h__
#define __tire_normal_h__

#include <limits>

#include "algebra/Vector.h"
#include "algebra/Matrix.h"

namespace tire {

template <typename T = float>
struct normal {
        using scalar_type = T;
        using self = normal<scalar_type>;
        using vec_type = tire::algebra::Vector3<scalar_type>;
        using mat3_type = tire::algebra::Matrix3<scalar_type>;
        using mat4_type = tire::algebra::Matrix4<scalar_type>;

        scalar_type x() {
            return normal_.x();
        }

        scalar_type y() {
            return normal_.y();
        }

        scalar_type z() {
            return normal_.z();
        }

        scalar_type dot(vec_type rhs) {
            return normal_.dot(rhs);
        }

        scalar_type squareLength() {
            return normal_.dot(normal_);
        }

        scalar_type lenght() {
            return normal_.length();
        }

        void normalize() {
            normal_.normalize();
        }

        void transform(mat3_type rhs) {
            auto i = rhs.inverse();
            auto ti = i.transpose();
            normal_ = ti.mult(normal_);
        }

    private:
        vec_type normal_;
};

using normali = normal<long long>;
using normalf = normal<float>;
using normald = normal<double>;

}  // namespace tire

#endif
