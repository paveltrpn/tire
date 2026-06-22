module;

#include "algebra/vector3.h"
#include "algebra/vector4.h"

export module geometry:bs;

using namespace tire::algebra;

namespace tire {

export struct BoundingSphere final {
    using value_type = float;

    BoundingSphere() = default;

    void setRadius( float value ) { radius_ = value; }
    void setPosition( vector3<value_type> value ) { position_ = value; }

    float radius() { return radius_; }
    vector3<value_type> position() { return position_; }

    bool testBoundingSphere( const BoundingSphere &other ) {
        vector3<value_type> dif = position_ - other.position_;
        value_type dist = dif.dot( dif );
        // Spheres intersect if squared distance is less than squared sum of radii
        value_type radiusSum = radius_ + other.radius_;
        return dist <= radiusSum * radiusSum;
    }

    // bool testAABoundingBox( const AABoundingBox<value_type> &other ) { return false; }

private:
    value_type radius_{};
    vector3<value_type> position_{};
};

}  // namespace tire
