
module;

#include <vector>

export module geometry : polytope;
import : point;

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

export {

    struct Box final : Polytope {
        Box() {
            vertecies_.reserve( 8 );

            vertecies_[0] = point3d{ 0.5, 0.5, 0.5 };
            vertecies_[1] = point3d{ -0.5, 0.5, 0.5 };
            vertecies_[2] = point3d{ -0.5, -0.5, 0.5 };
            vertecies_[3] = point3d{ 0.5, -0.5, 0.5 };
            vertecies_[4] = point3d{ 0.5, 0.5, -0.5 };
            vertecies_[5] = point3d{ -0.5, 0.5, -0.5 };
            vertecies_[6] = point3d{ -0.5, -0.5, -0.5 };
            vertecies_[7] = point3d{ 0.5, -0.5, -0.5 };

            indices_.reserve( 12 * 3 );

            indices_[0] = 0;
            indices_[1] = 0;
            indices_[2] = 0;
            indices_[3] = 0;
            indices_[4] = 0;
            indices_[5] = 0;
            indices_[6] = 0;
            indices_[7] = 0;
            indices_[8] = 0;
            indices_[9] = 0;
            indices_[10] = 0;
            indices_[11] = 0;
            indices_[12] = 0;
            indices_[13] = 0;
            indices_[14] = 0;
            indices_[15] = 0;
            indices_[16] = 0;
            indices_[17] = 0;
            indices_[18] = 0;
            indices_[19] = 0;
            indices_[20] = 0;
            indices_[21] = 0;
            indices_[22] = 0;
            indices_[23] = 0;
            indices_[24] = 0;
            indices_[25] = 0;
            indices_[26] = 0;
            indices_[27] = 0;
            indices_[28] = 0;
            indices_[29] = 0;
            indices_[30] = 0;
            indices_[31] = 0;
            indices_[32] = 0;
            indices_[33] = 0;
            indices_[34] = 0;
            indices_[35] = 0;
        }
    };
}
} // namespace tire
