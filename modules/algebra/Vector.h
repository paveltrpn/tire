
#ifndef __algebravector_h__
#define __algebravector_h__

#include "vector_base_self.h"

namespace tire::algebra {

template <typename T>
struct dummy_derived {};

template <typename T, size_t pSize_>
using Vector = vector_base<dummy_derived, T, pSize_>;

template <typename T>
using Vector2 = vector2<T>;

template <typename T>
using Vector3 = vector3<T>;

template <typename T>
using Vector4 = vector4<T>;

using Vector2i = Vector2<int>;
using Vector3i = Vector3<int>;
using Vector4i = Vector4<int>;

using Vector2ui = Vector2<unsigned int>;
using Vector3ui = Vector3<unsigned int>;
using Vector4ui = Vector4<unsigned int>;

using Vector2l = Vector2<long long>;
using Vector3l = Vector3<long long>;
using Vector4l = Vector4<long long>;

using Vector2f = Vector2<float>;
using Vector3f = Vector3<float>;
using Vector4f = Vector4<float>;

using Vector2d = Vector2<double>;
using Vector3d = Vector3<double>;
using Vector4d = Vector4<double>;

template <typename T>
concept ConceptVector
  = std::is_same_v<T, Vector4l> || std::is_same_v<T, Vector3f> || std::is_same_v<T, Vector4d>
    || std::is_same_v<T, Vector4i> || std::is_same_v<T, Vector4ui> || std::is_same_v<T, Vector3l>
    || std::is_same_v<T, Vector3f> || std::is_same_v<T, Vector3d> || std::is_same_v<T, Vector3i>
    || std::is_same_v<T, Vector3ui> || std::is_same_v<T, Vector2l> || std::is_same_v<T, Vector2f>
    || std::is_same_v<T, Vector2d> || std::is_same_v<T, Vector2i> || std::is_same_v<T, Vector2ui>;
}  // namespace tire::algebra

#endif
