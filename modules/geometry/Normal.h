
#ifndef __tire_normal_h__
#define __tire_normal_h__

#include "glm/vec3.hpp"

namespace tire {

template <typename T>
struct normal {
    private:
        glm::vec<3, T> normal_;
};

using normali = normal<long long>;
using normalf = normal<float>;
using normald = normal<double>;

}  // namespace tire

#endif
