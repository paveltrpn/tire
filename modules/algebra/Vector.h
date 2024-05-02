
#ifndef __algebravector_h__
#define __algebravector_h__

#include "vector_base_glm.h"

namespace tire::algebra {

template <typename T, size_t size_>
using Vector = vector_base_glm<T, size_>;

template <typename T>
using Vector2 = Vector<T, 2>;

template <typename T>
using Vector3 = Vector<T, 3>;

template <typename T>
using Vector4 = Vector<T, 4>;

using Vector2i = Vector2<long long>;
using Vector3i = Vector3<long long>;
using Vector4i = Vector4<long long>;

using Vector2f = Vector2<float>;
using Vector3f = Vector3<float>;
using Vector4f = Vector4<float>;

using Vector2d = Vector2<double>;
using Vector3d = Vector3<double>;
using Vector4d = Vector4<double>;

}  // namespace tire::algebra

#endif
