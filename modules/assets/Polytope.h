
#ifndef __assets_polytope_h__
#define __assets_polytope_h__

#include <iostream>
#include <string>
#include <vector>

namespace tire {

enum class PolytopeType : char {
    TETRAHEDRON,
    OCTAHEDRON,
    DODECAHEDRON,
    ICOSAHEDRON,
    BOX,
    SPHERE,
    TOURUS,
    TEAPOT
};

struct Polytope final {
        Polytope(PolytopeType type);

    private:
};

}  // namespace tire

#endif
