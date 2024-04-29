
#ifndef __tire_geometry_vector_h__
#define __tire_geometry_vector_h__

#include <glm/vec3.hpp>

namespace tire {

template <size_t size_, typename T>
using vec = glm::vec<size_, T>;

template <size_t size_, typename T>
using vec2 = glm::vec<2, T>;

template <size_t size_, typename T>
using vec3 = glm::vec<3, T>;

}  // namespace tire

#endif
