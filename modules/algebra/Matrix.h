
#ifndef __algebramatrix_h__
#define __algebramatrix_h__

#include <cstddef>

#include "matrix_base_glm.h"
#include "Vector.h"

namespace tire::algebra {

template <typename T, size_t size_>
using Matrix = matrix_base_glm<T, size_>;

template <typename T>
using Matrix2 = matrix_base_glm<T, 2>;

template <typename T>
using Matrix3 = matrix_base_glm<T, 3>;

template <typename T>
using Matrix4 = matrix_base_glm<T, 4>;

using Matrix2i = Matrix2<long long>;
using Matrix3i = Matrix3<long long>;
using Matrix4i = Matrix4<long long>;

using Matrix2f = Matrix2<float>;
using Matrix3f = Matrix3<float>;
using Matrix4f = Matrix4<float>;

using Matrix2d = Matrix2<double>;
using Matrix3d = Matrix3<double>;
using Matrix4d = Matrix4<double>;

}  // namespace tire::algebra

#endif
