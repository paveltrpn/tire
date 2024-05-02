
#ifndef __matrix_base_glm_h__
#define __matrix_base_glm_h__

#include "Vector.h"

namespace tire::algebra {

template <typename T, size_t size_>
struct matrix_base_glm {
        using scalar_type = T;
        using self = matrix_base_glm<T, size_>;

    private:
        glm::mat<size_, size_, scalar_type> mat_;
};

template <typename T>
struct matrix_base_glm<T, 2> {
        using scalar_type = T;
        using vec_type = Vector<scalar_type, 2>;
        using self = matrix_base_glm<scalar_type, 2>;

        matrix_base_glm() = default;
        matrix_base_glm(vec_type r1, vec_type r2)
            : mat_{ glm::vec<2, scalar_type>{ r1.x(), r1.y() },
                    glm::vec<2, scalar_type>{ r2.x(), r2.y() } } {
        }

    private:
        glm::mat<2, 2, scalar_type> mat_;
};

template <typename T>
struct matrix_base_glm<T, 3> {
        using scalar_type = T;
        using vec_type = Vector<scalar_type, 3>;
        using self = matrix_base_glm<scalar_type, 3>;

        matrix_base_glm() = default;
        matrix_base_glm(vec_type r1, vec_type r2, vec_type r3)
            : mat_{ glm::vec<3, scalar_type>{ r1.x(), r1.y(), r1.x() },
                    glm::vec<3, scalar_type>{ r2.x(), r2.y(), r1.z() },
                    glm::vec<3, scalar_type>{ r3.x(), r3.y(), r3.z() } } {
        }

    private:
        glm::mat<3, 3, scalar_type> mat_;
};

template <typename T>
struct matrix_base_glm<T, 4> {
        using scalar_type = T;
        using vec_type = Vector<scalar_type, 3>;
        using self = matrix_base_glm<scalar_type, 4>;

        matrix_base_glm() = default;
        matrix_base_glm(vec_type r1, vec_type r2, vec_type r3, vec_type r4)
            : mat_{ glm::vec<4, scalar_type>{ r1.x(), r1.y(), r1.x(), r1.w() },
                    glm::vec<4, scalar_type>{ r2.x(), r2.y(), r1.z(), r2.w() },
                    glm::vec<4, scalar_type>{ r3.x(), r3.y(), r3.z(), r3.w() },
                    glm::vec<4, scalar_type>{ r4.x(), r4.y(), r4.z(), r4.w() } } {
        }

    private:
        glm::mat<4, 4, scalar_type> mat_;
};

}  // namespace tire::algebra

#endif
