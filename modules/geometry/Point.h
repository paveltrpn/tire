
#ifndef __tire_point_h__
#define __tire_point_h__

#include "glm/ext/vector_float3.hpp"
#include "glm/vec3.hpp"

namespace tire {

template <typename T>
struct point2 {
    private:
        glm::vec<2, T> pos_;
};

template <typename T>
struct point3 {
    private:
        glm::vec<3, T> pos_;
};

template <typename T>
struct point4 {
    private:
        glm::vec<4, T> pos_;
};

using point2i = point2<long long>;
using point2f = point2<float>;
using point2d = point2<double>;

using point3i = point3<long long>;
using point3f = point3<float>;
using point3d = point3<double>;

using point4i = point4<long long>;
using point4f = point4<float>;
using point4d = point4<double>;

}  // namespace tire

#endif