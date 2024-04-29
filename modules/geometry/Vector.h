
#ifndef __tire_geometry_vector_h__
#define __tire_geometry_vector_h__

#include <glm/vec3.hpp>

namespace tire {

template <size_t size_, typename T>
using vec = glm::vec<size_, T>;

template <typename T>
using vec2 = vec<2, T>;

template <typename T>
using vec3 = vec<3, T>;

template <typename T>
using vec4 = vec<4, T>;

using vec2i = vec2<long long>;
using vec3i = vec3<long long>;
using vec4i = vec4<long long>;

using vec2f = vec2<float>;
using vec3f = vec3<float>;
using vec4f = vec4<float>;

using vec2d = vec2<double>;
using vec3d = vec3<double>;
using vec4d = vec4<double>;

}  // namespace tire

#endif
