module;

#include <array>

export module geometry:oobb;

import algebra;

import :bs;
import :aabb;

using namespace tire::algebra;

namespace tire {

export struct OOBoundingBox final {
    using value_type = float;

    OOBoundingBox() = default;

    // Construct OOBB from transformed AABB. In other
    // words - initialize OOBB from AABB and transformation matrix.
    OOBoundingBox( const AABoundingBox &other, const matrix4<value_type> &transformation ) {}

private:
    // OOBB center point
    vector3<value_type> center_{};
    // Local x-, y-, and z-axes
    std::array<vector3<value_type>, 3> local_{};
    // Positive halfwidth extents of OBB along each axis
    vector3<value_type> extents_{};
};

}  // namespace tire
