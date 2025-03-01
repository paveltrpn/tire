
#pragma once

#include "algebra/vector3.h"

namespace tire {

struct BoundingSphere {
    void setRadius( float value ) { radius_ = value; }
    void setPosition( algebra::vector3f value ) { position_ = value; }
    float radious() { return radius_; }
    algebra::vector3f position() { return position_; }

private:
    float radius_{};
    algebra::vector3f position_{};
};

struct AABoundingBox {
private:
    algebra::vector3f min_{};
    algebra::vector3f max_{};
};

struct OOBoundingBox {
    void setMin( algebra::vector3f value ) { min_ = value; }
    void setMax( algebra::vector3f value ) { max_ = value; }

    algebra::vector3f min() { return min_; };
    algebra::vector3f max() { return max_; };

private:
    algebra::vector3f min_{};
    algebra::vector3f max_{};
};

}  // namespace tire