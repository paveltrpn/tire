
#pragma once

#include <vector>
#include <initializer_list>
#include <iterator>

#include "polytope.h"
#include "point.h"
#include "normal.h"

import toy_std;

using namespace toy::algebra;

namespace tire
{
struct Node {
    Node() = default;

    Node( const Polytope &body );

    size_t getVerteciesCount() const;
    size_t getVerteciesArraySize() const;

    point3d *getVerteciesData();
    long long *getIndeciessData();

    void setOffset( vector3d offst );
    void setRotate( matrix3d rtn );
    void setScale( matrix3d scl );
    void applyRotate();
    void applyScale();

private:
    bool dirty_{ false };
    bool useIndecies_{ false };

    std::vector<point3d> vertecies_;
    std::vector<long long> indices_;
    std::vector<vector3f> colors_;
    std::vector<vector2f> texCoords_;
    std::vector<normald> normals_;

    vector3d offset_{};
    matrix3d rotation_{};
    matrix3d scale_{};
};
} // namespace tire
