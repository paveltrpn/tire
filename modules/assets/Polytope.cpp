
module;

#include <iostream>
#include <string>
#include <vector>

export module assets:polytope;

namespace tire {

export {
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
            Polytope(PolytopeType type) {
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

        private:
    };
}
}  // namespace tire
