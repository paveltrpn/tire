
#ifndef __tire_normal_h__
#define __tire_normal_h__

#include <limits>
#include "glm/matrix.hpp"
#include "glm/vec3.hpp"
#include "glm/mat3x3.hpp"

namespace tire {

template <typename T = float>
struct normal {
        using self = normal<T>;
        using vec_type = glm::vec<3, T>;
        using mat3x3_type = glm::mat<3, 3, T>;
        using mat4x4_type = glm::mat<4, 4, T>;

        T dot(vec_type rhs) {
            return normal_.x * rhs.x + normal_.y * rhs.y + normal_.z * rhs.z;
        }

        T sqLength() {
            return dot(this->normal_);
        }

        T lenght() {
            return std::sqrt(sqLength());
        }

        void normalize() {
            auto l = lenght();
            if (l < std::numeric_limits<T>::epsilon()) {
                return;
            } else {
                normal_.x /= l;
                normal_.y /= l;
                normal_.z /= l;
            }
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
