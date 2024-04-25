
#ifndef __tire_point_h__
#define __tire_point_h__

#include "glm/ext/vector_float3.hpp"
#include "glm/vec3.hpp"

namespace tire {

template <typename scalarT_ = float>
struct point3 {
        using self = point3<scalarT_>;
        using vec_type = glm::vec<3, scalarT_>;
        using mat3x3_type = glm::mat<3, 3, scalarT_>;
        using mat4x4_type = glm::mat<4, 4, scalarT_>;

        point3() : pos_{} {
        }

        point3(scalarT_ x, scalarT_ y, scalarT_ z) : pos_{ x, y, z } {
        }

        point3(vec_type rhs) : pos_{ rhs } {
        }

        scalarT_ x() {
            return pos_.x();
        }

        scalarT_ y() {
            return pos_.y();
        }

        scalarT_ z() {
            return pos_.z();
        }

        void move(vec_type offst) {
            pos_ += offst;
        }

        void transform(mat3x3_type factor) {
            pos_ *= factor;
        }

        void transform(mat4x4_type factor) {
            pos_ *= factor;
        }

        self& operator+(vec_type offst) {
            pos_ = pos_ + offst;
            return *this;
        }

        self& operator-(vec_type offst) {
            pos_ = pos_ - offst;
            return *this;
        }

        self& operator+=(vec_type offst) {
            pos_ += offst;
            return *this;
        }

        self& operator-=(vec_type offst) {
            pos_ -= offst;
            return *this;
        }

        self& operator*(mat3x3_type factor) {
            pos_ = pos_ * factor;
            return *this;
        }

        self& operator*=(mat3x3_type factor) {
            pos_ = pos_ * factor;
        }

        self& operator*(mat4x4_type factor) {
            pos_ = pos_ * factor;
            return *this;
        }

        self& operator*=(mat4x4_type factor) {
            pos_ = pos_ * factor;
        }

    private:
        vec_type pos_;
};

using point3i = point3<long long>;
using point3f = point3<float>;
using point3d = point3<double>;

}  // namespace tire

#endif
