
#ifndef __tire_normal_h__
#define __tire_normal_h__

#include <limits>

#include "Vector.h"
#include "Matrix.h"

namespace tire {

template <typename scalarT_ = float>
struct normal {
        using self = normal<scalarT_>;
        using vec_type = tire::vec<3, scalarT_>;
        using mat3x3_type = tire::mat<3, scalarT_>;
        using mat4x4_type = tire::mat<4, scalarT_>;

        scalarT_ x() {
            return normal_.x();
        }

        scalarT_ y() {
            return normal_.y();
        }

        scalarT_ z() {
            return normal_.z();
        }

        scalarT_ dot(vec_type rhs) {
            return glm::dot(normal_, rhs);
        }

        scalarT_ squareLength() {
            return glm::dot(this->normal_, this->normal_);
        }

        scalarT_ lenght() {
            return normal_.lenght();
        }

        void normalize() {
            glm::normalize(normal_);
        }

        void transform(mat3x3_type rhs) {
            auto i_rhs = glm::inverse(rhs);
            normal_ *= glm::transpose(i_rhs);
        }

        void transform(mat4x4_type rhs) {
            auto i_rhs = glm::inverse(rhs);
            normal_ *= glm::transpose(i_rhs);
        }

    private:
        vec_type normal_;
};

using normali = normal<long long>;
using normalf = normal<float>;
using normald = normal<double>;

}  // namespace tire

#endif
