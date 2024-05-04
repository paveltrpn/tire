
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
  = std::is_same_v<T, Vector<long long, 4>> || std::is_same_v<T, Vector<float, 4>>
    || std::is_same_v<T, Vector<double, 4>> || std::is_same_v<T, Vector<int, 4>>
    || std::is_same_v<T, Vector<unsigned int, 4>> || std::is_same_v<T, Vector<long long, 3>>
    || std::is_same_v<T, Vector<float, 3>> || std::is_same_v<T, Vector<double, 3>>
    || std::is_same_v<T, Vector<int, 3>> || std::is_same_v<T, Vector<unsigned int, 3>>
    || std::is_same_v<T, Vector<long long, 2>> || std::is_same_v<T, Vector<float, 2>>
    || std::is_same_v<T, Vector<double, 2>> || std::is_same_v<T, Vector<int, 2>>
    || std::is_same_v<T, Vector<unsigned int, 2>>;
}  // namespace tire::algebra

#endif
