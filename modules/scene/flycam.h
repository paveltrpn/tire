

#pragma once

#include "algebra/vector3.h"
#include "geometry/bounding_volumes.h"

namespace tire {

struct Flycam final {
private:
    algebra::vector3f position_{};
    float yaw_{};
    float pitch_{};
    float roll_{};

    BoundingSphere<float> bounding_{};
    float mass_{};
    algebra::vector3f velocity_{};
};

}  // namespace tire