
#ifndef __matrix_base_glm_h__
#define __matrix_base_glm_h__

#include "Vector.h"

#include <glm/gtc/matrix_transform.hpp>

namespace tire::algebra {

template <typename T, size_t size_>
struct matrix_base_glm {
        using scalar_type = T;
        using self = matrix_base_glm<T, size_>;

        glm::mat<size_, size_, scalar_type> mat_;
};

template <typename T>
struct matrix_base_glm<T, 2> {
        using scalar_type = T;
        using vec_type = Vector2<scalar_type>;
        using self = matrix_base_glm<scalar_type, 2>;

        matrix_base_glm() = default;
        matrix_base_glm(vec_type r1, vec_type r2)
            : mat_{ glm::vec<2, scalar_type>{ r1.x(), r1.y() },
                    glm::vec<2, scalar_type>{ r2.x(), r2.y() } } {
        }

        glm::mat<2, 2, scalar_type> mat_;
};

template <typename T>
struct matrix_base_glm<T, 3> {
        using scalar_type = T;
        using vec_type = Vector3<scalar_type>;
        using self = matrix_base_glm<scalar_type, 3>;

        matrix_base_glm() = default;
        matrix_base_glm(const glm::mat<3, 3, scalar_type> &rhs) : mat_{ rhs } {
        }
        matrix_base_glm(vec_type r1, vec_type r2, vec_type r3)
            : mat_{ glm::vec<3, scalar_type>{ r1.x(), r1.y(), r1.z() },
                    glm::vec<3, scalar_type>{ r2.x(), r2.y(), r1.z() },
                    glm::vec<3, scalar_type>{ r3.x(), r3.y(), r3.z() } } {
        }

        [[nodiscard]]
        self mult(const self &rhs) {
            return mat_ * rhs.mat_;
        }

        [[nodiscard]]
        vec_type mult(const vec_type &rhs) {
            auto rt = mat_ * rhs.vec_;
            return { rt.x, rt.y, rt.z };
        }

        [[nodiscard]]
        self inverse() {
            return { glm::inverse(mat_) };
        }

        [[nodiscard]]
        self transpose() {
            return { glm::transpose(mat_) };
        }

        glm::mat<3, 3, scalar_type> mat_;
};

template <typename T>
struct matrix_base_glm<T, 4> {
        using scalar_type = T;
        using vec_type = Vector4<scalar_type>;
        using self = matrix_base_glm<scalar_type, 4>;

        matrix_base_glm() = default;
        matrix_base_glm(const glm::mat<4, 4, scalar_type> &rhs) : mat_{ rhs } {
        }
        matrix_base_glm(vec_type r1, vec_type r2, vec_type r3, vec_type r4)
            : mat_{ glm::vec<4, scalar_type>{ r1.x(), r1.y(), r1.z(), r1.w() },
                    glm::vec<4, scalar_type>{ r2.x(), r2.y(), r1.z(), r2.w() },
                    glm::vec<4, scalar_type>{ r3.x(), r3.y(), r3.z(), r3.w() },
                    glm::vec<4, scalar_type>{ r4.x(), r4.y(), r4.z(), r4.w() } } {
        }

        [[nodiscard]]
        self mult(const self &rhs) {
            return mat_ * rhs.mat_;
        }

        [[nodiscard]]
        vec_type mult(const vec_type &rhs) {
            auto rt = mat_ * rhs.vec_;
            return { rt.x, rt.y, rt.z, rt.w };
        }

        [[nodiscard]]
        self inverse() {
            return { glm::inverse(mat_) };
        }

        [[nodiscard]]
        self transpose() {
            return { glm::transpose(mat_) };
        }

        void translate(scalar_type dx, scalar_type dy, scalar_type dz) {
            mat_ = glm::translate(glm::mat4{ 1.0f }, glm::vec<3, scalar_type>{ dx, dy, dz });
        }

        void perspective(float fov, float aspect, float ncp, float fcp) {
            mat_ = glm::perspective(fov, aspect, ncp, fcp);
        }

        glm::mat<4, 4, scalar_type> mat_;
};

}  // namespace tire::algebra

#endif
