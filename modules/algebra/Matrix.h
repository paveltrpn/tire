
#ifndef __algebramatrix_h__
#define __algebramatrix_h__

#include <cstddef>
#include <type_traits>

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

using Matrix2i = Matrix2<int>;
using Matrix3i = Matrix3<int>;
using Matrix4i = Matrix4<int>;

using Matrix2ui = Matrix2<unsigned int>;
using Matrix3ui = Matrix3<unsigned int>;
using Matrix4ui = Matrix4<unsigned int>;

using Matrix2ll = Matrix2<long long>;
using Matrix3ll = Matrix3<long long>;
using Matrix4ll = Matrix4<long long>;

using Matrix2f = Matrix2<float>;
using Matrix3f = Matrix3<float>;
using Matrix4f = Matrix4<float>;

using Matrix2d = Matrix2<double>;
using Matrix3d = Matrix3<double>;
using Matrix4d = Matrix4<double>;

template <typename T>
concept ConceptMatrixFloat
  = std::is_same_v<T, Matrix<float, 2>> || std::is_same_v<T, Matrix<float, 3>>
    || std::is_same_v<T, Matrix<float, 4>>;

template <typename T>
concept ConceptMatrix
  = std::is_same_v<T, Matrix<long long, 4>> || std::is_same_v<T, Matrix<float, 4>>
    || std::is_same_v<T, Matrix<double, 4>> || std::is_same_v<T, Matrix<int, 4>>
    || std::is_same_v<T, Matrix<unsigned int, 4>> || std::is_same_v<T, Matrix<long long, 3>>
    || std::is_same_v<T, Matrix<float, 3>> || std::is_same_v<T, Matrix<double, 3>>
    || std::is_same_v<T, Matrix<int, 3>> || std::is_same_v<T, Matrix<unsigned int, 3>>
    || std::is_same_v<T, Matrix<long long, 2>> || std::is_same_v<T, Matrix<float, 2>>
    || std::is_same_v<T, Matrix<double, 2>> || std::is_same_v<T, Matrix<int, 2>>
    || std::is_same_v<T, Matrix<unsigned int, 2>>;

}  // namespace tire::algebra

#endif
