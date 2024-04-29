
#ifndef __tire_geometry_matrix_h__
#define __tire_geometry_matrix_h__

#include <glm/matrix.hpp>

namespace tire {

template <size_t size_, typename T>
using mat = glm::mat<size_, size_, T>;

template <typename T>
using mat2 = mat<2, T>;

template <typename T>
using mat3 = mat<3, T>;

template <typename T>
using mat4 = mat<4, T>;

using mat2i = mat2<long long>;
using mat3i = mat3<long long>;
using mat4i = mat4<long long>;

using mat2f = mat2<float>;
using mat3f = mat3<float>;
using mat4f = mat4<float>;

using mat2d = mat2<double>;
using mat3d = mat3<double>;
using mat4d = mat4<double>;

}  // namespace tire

#endif
