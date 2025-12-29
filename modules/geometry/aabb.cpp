module;

export module geometry:aabb;

import algebra;

import :bs;

using namespace tire::algebra;

namespace tire {

export struct AABoundingBox final {
    using value_type = float;

    AABoundingBox() = default;

    AABoundingBox( vector3<value_type> min, vector3<value_type> max )
        : min_{ min }
        , max_{ max } {};

    // Construct surrounding AABB from given transformed AABB.
    AABoundingBox( const AABoundingBox &other, const matrix4<value_type> &transformation ) {}

    bool testBoundingSphere( const BoundingSphere &other ) {
        //
        return false;
    }

    bool testAABoundingBox( const AABoundingBox &other ) {
        // Exit with no intersection if separated along an axis
        if ( max_.x() < other.min_.x() || min_.x() > other.max_.x() ) return false;
        if ( max_.y() < other.min_.y() || min_.y() > other.max_.y() ) return false;
        if ( max_.z() < other.min_.z() || min_.z() > other.max_.z() ) return false;
        // Overlapping on all axes means AABBs are intersecting
        return true;
    }

private:
    vector3<value_type> min_{};
    vector3<value_type> max_{};
};

}  // namespace tire
