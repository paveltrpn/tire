
#pragma once

#include "algebra/vector3.h"

namespace tire {

// NOTE: introduce namespace!
using namespace algebra;

template <typename T>
struct CollisionShape {};

template <typename T>
struct BoundingSphere final : CollisionShape<T> {
    using value_type = T;

    void setRadius( float value ) { radius_ = value; }
    void setPosition( vector3<value_type> value ) { position_ = value; }

    float radius() { return radius_; }
    vector3<value_type> position() { return position_; }

private:
    value_type radius_{};
    vector3<value_type> position_{};
};

template <typename T>
struct AABoundingBox final : CollisionShape<T> {
    using value_type = T;

private:
    vector3<value_type> min_{};
    vector3<value_type> max_{};
};

template <typename T>
struct OOBoundingBox final : CollisionShape<T> {
    using value_type = T;

    void setMin( vector3<value_type> value ) { min_ = value; }
    void setMax( vector3<value_type> value ) { max_ = value; }

    vector3<value_type> min() { return min_; };
    vector3<value_type> max() { return max_; };

private:
    vector3<value_type> min_{};
    vector3<value_type> max_{};
};

}  // namespace tire