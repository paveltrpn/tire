
#pragma once

#include <vector>
#include <initializer_list>
#include <iterator>

#include "polytope.h"
#include "point.h"
#include "normal.h"

#include "algebra/vector2.h"
#include "algebra/vector3.h"
#include "algebra/matrix3.h"

namespace tire
{
struct Node {
    Node() = default;

    Node( const Polytope &body );

    size_t getVerteciesCount() const;
    size_t getVerteciesArraySize() const;

    point3d *getVerteciesData();
    long long *getIndeciessData();

    void setOffset( algebra::vector3d offst );
    void setRotate( algebra::matrix3d rtn );
    void setScale( algebra::matrix3d scl );
    void applyRotate();
    void applyScale();

private:
    bool dirty_{ false };
    bool useIndecies_{ false };

    std::vector<point3d> vertecies_;
    std::vector<long long> indices_;
    std::vector<algebra::vector3f> colors_;
    std::vector<algebra::vector2f> texCoords_;
    std::vector<normald> normals_;

    algebra::vector3d offset_{};
    algebra::matrix3d rotation_{};
    algebra::matrix3d scale_{};
};
} // namespace tire
