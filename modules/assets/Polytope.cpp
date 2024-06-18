
#include "Polytope.h"

namespace tire {

Polytope::Polytope(PolytopeType type) {
    switch (type) {
    case tire::PolytopeType::TETRAHEDRON: {
        break;
    }
    case tire::PolytopeType::OCTAHEDRON: {
        break;
    }
    case tire::PolytopeType::DODECAHEDRON: {
        break;
    }
    case tire::PolytopeType::ICOSAHEDRON: {
        break;
    }
    case tire::PolytopeType::BOX: {
        break;
    }
    case tire::PolytopeType::SPHERE: {
        break;
    }
    case tire::PolytopeType::TOURUS: {
        break;
    }
    case tire::PolytopeType::TEAPOT: {
        break;
    }
    default:
        break;
    }
}

}  // namespace tire
