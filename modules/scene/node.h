
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

namespace tire {

struct Node final : std::enable_shared_from_this<Node> {
    Node( const Polytope &body );

    std::shared_ptr<const Node> asSharedPtr() const;
    std::shared_ptr<Node> asSharedPtr();

    size_t verteciesCount() const;
    size_t verteciesArraySize() const;

    size_t indeciesCount() const;
    size_t indeciesArraySize() const;

    long long getTrianglesCount() { return trianglesCount_; };

    point3d *verteciesData();
    unsigned int *indeciesData();

    void setOffset( algebra::vector3d offst );
    void setRotate( algebra::matrix3d rtn );
    void setScale( algebra::matrix3d scl );
    void applyRotate();
    void applyScale();

private:
    bool dirty_{ false };

    long long trianglesCount_{};

    std::vector<point3d> vertecies_;
    std::vector<unsigned int> indices_;
    std::vector<algebra::vector3f> colors_;
    std::vector<algebra::vector2f> texCoords_;
    std::vector<normald> normals_;

    algebra::vector3d offset_{};
    algebra::matrix3d rotation_{};
    algebra::matrix3d scale_{};
};

}  // namespace tire
