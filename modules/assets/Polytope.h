
#ifndef __assets_polytope_h__
#define __assets_polytope_h__

#include <iostream>
#include <string>
#include <vector>

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

struct polytope {
        polytope(PolytopeType type);

    private:
};

}  // namespace tire

#endif
