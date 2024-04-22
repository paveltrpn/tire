
#ifndef __tire_point_h__
#define __tire_point_h__

#include "glm/ext/vector_float3.hpp"
#include "glm/vec3.hpp"

namespace tire {

template <typename T>
struct point3 {
        using value_type = T;
        
        using self = point3<T>;
        using vec_type = glm::vec<3, T>;
        using mat3x3_type = glm::mat<3, 3, T>;
        using mat4x4_type = glm::mat<4, 4, T>;

        point3() : pos_{} {
        }

        point3(T x, T y, T z) : pos_{ x, y, z } {
        }

        point3(vec_type rhs) : pos_{ rhs } {
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