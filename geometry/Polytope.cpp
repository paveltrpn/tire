
module;

#include <iostream>
#include <string>
#include <vector>

export module geometry:Polythope;

import toy_std;

import :Basics;

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
        std::vector<point3<float>> vertecies_;
        std::vector<int> indexes_;
        std::vector<normal3<float>> normals_;

        toy::algebra::vector3f baseColor_;
};

}  // namespace tire
