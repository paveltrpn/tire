
#pragma once

#include <vector>

#include "point.h"

namespace tire {

struct Polytope {
    Polytope() = default;

    std::vector<point3d> getVertecies() const { return vertecies_; };
    std::vector<unsigned int> getIndices() const { return indices_; };

protected:
    std::vector<point3d> vertecies_;
    std::vector<unsigned int> indices_;
};

struct Box final : Polytope {
    Box();
};

struct Sphere final : Polytope {
    Sphere();
};

}  // namespace tire
