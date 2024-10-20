
#pragma once

#include <vector>

#include "point.h"

namespace tire
{

enum class PolytopeType : char
{
    TETRAHEDRON,
    OCTAHEDRON,
    DODECAHEDRON,
    ICOSAHEDRON,
    BOX,
    SPHERE,
    TOURUS,
    TEAPOT
};

struct Polytope {
    Polytope() = default;

    std::vector<point3d> getVertecies() const { return vertecies_; };
    std::vector<long long> getIndices() const { return indices_; };

protected:
    std::vector<point3d> vertecies_;
    std::vector<long long> indices_;
};

struct Box final : Polytope {
    Box();
};

struct Sphere final : Polytope {
    Sphere();
};

} // namespace tire
