
#ifndef __geometry_polytope_h__
#define __geometry_polytope_h__

#include <iostream>
#include <string>
#include <vector>

#include "Basics.h"

namespace tire {

enum class PolytopeType {
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
        Polytope(PolytopeType type);

    private:
        std::vector<point3<float>> vertecies_;
        std::vector<int> indexes_;
        std::vector<normal3<float>> normals_;

        toy::algebra::vector3f baseColor_;
};

}  // namespace tire

#endif
