module;

export module geometry:bs;

import algebra;

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
