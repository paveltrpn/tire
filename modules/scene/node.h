
#pragma once

#include <vector>
#include <initializer_list>
#include <iterator>
#include <memory>

#include "geometry/polytope.h"
#include "geometry/point.h"
#include "geometry/normal.h"

#include "algebra/vector2.h"
#include "algebra/vector3.h"
#include "algebra/matrix3.h"

#include "image/color.h"

namespace tire {

struct Node final : std::enable_shared_from_this<Node> {
    Node( const Polytope &body );

    std::shared_ptr<const Node> asSharedPtr() const;
    std::shared_ptr<Node> asSharedPtr();

    size_t verteciesCount() const;
    size_t indeciesCount() const;
    size_t verteciesArraySize() const;
    size_t indeciesArraySize() const;
    point3f *verteciesData();
    unsigned int *indeciesData();

    long long getTrianglesCount() { return trianglesCount_; };

    void setColor( const std::string &name ) { color_ = Colorf{ name }; }
    Colorf color() const { return color_; };

    void setPivotOffset( algebra::vector3f offst );
    void setPivotRotation( algebra::vector3f rtn );
    void setPivotScale( algebra::vector3f scl );
    void setUseMomentum( bool use ) { useMomentum_ = use; }
    bool useMomentum() { return useMomentum_; }
    void setMomentum( algebra::vector3f rtn );

    void applyPivotTransormations();
    void applyInversePivotTransormations();

private:
    bool dirty_{ false };

    long long trianglesCount_{};

    // Store default object vertecies and normal instead of inverse transformations
    std::vector<point3f> defaultVertecies_;
    std::vector<point3f> vertecies_;
    std::vector<normald> defaultNormals_;
    std::vector<normald> normals_;

    std::vector<unsigned int> indices_;
    std::vector<algebra::vector2f> texCoords_;

    Colorf color_{};

    algebra::matrix4f offset_{};
    algebra::matrix4f rotation_{};
    algebra::matrix4f scale_{};
    bool useMomentum_{};
    algebra::matrix4f momentum_{};
};

}  // namespace tire
