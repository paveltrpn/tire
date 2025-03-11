

#pragma once

#include "algebra/vector3.h"
#include "geometry/bounding_volumes.h"

namespace tire {

struct Flycam final {
private:
    algebra::vector3f position_{};
    algebra::vector3f look_{};
    algebra::vector3f left_{};
    // Default up vector +Y
    algebra::vector3f up_{ 0.0f, 1.0f, 0.0f };

    float yaw_{};
    float pitch_{};
    float roll_{};

    BoundingSphere<float> bounding_{};
    float mass_{};
    algebra::vector3f velocity_{};
};

}  // namespace tire