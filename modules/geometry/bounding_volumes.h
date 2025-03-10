
#pragma once

#include "algebra/vector3.h"

namespace tire {

// NOTE: introduce namespace!
using namespace algebra;

template <typename T>
struct BoundingVolume {};

template <typename T>
struct AABoundingBox;

template <typename T>
struct OOBoundingBox;

template <typename T>
struct BoundingSphere final : BoundingVolume<T> {
    using value_type = T;

    void setRadius( float value ) { radius_ = value; }
    void setPosition( vector3<value_type> value ) { position_ = value; }

    float radius() { return radius_; }
    vector3<value_type> position() { return position_; }

    bool testBoundingSphere( const BoundingSphere<value_type> &other ) {
        vector3<value_type> dif = position_ - other.position_;
        value_type dist = dif * dif;
        // Spheres intersect if squared distance is less than squared sum of radii
        float radiusSum = radius_ + other.radius_;
        return dist <= radiusSum * radiusSum;
    }

    bool testAABoundingBox( const AABoundingBox<value_type> &other ) {}

private:
    value_type radius_{};
    vector3<value_type> position_{};
};

template <typename T>
struct AABoundingBox final : BoundingVolume<T> {
    using value_type = T;

    bool testBoundingSphere( const BoundingSphere<value_type> &other ) {}
    bool testAABoundingBox( const AABoundingBox<value_type> &other ) {
        // Exit with no intersection if separated along an axis
        if ( max_[0] < other.min_[0] || min_[0] > other.max_[0] ) return false;
        if ( max_[1] < other.min_[1] || min_[1] > other.max_[1] ) return false;
        if ( max_[2] < other.min_[2] || min_[2] > other.max_[2] ) return false;
        // Overlapping on all axes means AABBs are intersecting
        return true;
    }

private:
    vector3<value_type> min_{};
    vector3<value_type> max_{};
};

template <typename T>
struct OOBoundingBox final : BoundingVolume<T> {
    using value_type = T;

private:
    // OOBB center point
    vector3<value_type> center_{};
    // Local x-, y-, and z-axes
    std::array<vector3<value_type>, 3> local_{};
    // Positive halfwidth extents of OBB along each axis
    vector3<value_type> extents_{};
};

}  // namespace tire