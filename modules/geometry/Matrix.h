
#ifndef __tire_geometry_matrix_h__
#define __tire_geometry_matrix_h__

# include <glm/matrix.hpp>

namespace tire {

template <size_t size_, typename T>
using mat = glm::mat<size_, size_, T>;

template <typename T>
using mat2 = glm::mat<2, 2, T>;

template <typename T>
using mat3 = glm::mat<3, 3, T>;

template <typename T>
using mat4 = glm::mat<4, 4, T>;

}  // namespace tire

#endif
