
module;

#include <iostream>
#include <string>
#include <vector>

export module geometry;

import toy_std;

namespace tire {

export enum class PolytopeType {
    TETRAHEDRON,
    OCTAHEDRON,
    DODECAHEDRON,
    ICOSAHEDRON,
    BOX,
    SPHERE,
    TOURUS,
    TEAPOT
};

export struct Polytope {
        Polytope(PolytopeType type) {
        }

    private:
        std::vector<toy::algebra::vector3f> verticies_;
        std::vector<int> indexes_;
        std::vector<toy::algebra::vector3f> normals_;

        toy::algebra::vector3f baseColor_;
};

}  // namespace tire
