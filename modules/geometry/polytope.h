
#pragma once

#include <vector>

#include "point.h"

namespace tire {

struct Polytope {
    Polytope() = default;

    [[nodiscard]] std::vector<point3f> getVertecies() const {
        return vertecies_;
    };

    [[nodiscard]] std::vector<unsigned int> getIndices() const {
        return indices_;
    };

    [[nodiscard]] long long getTrianglesCount() const {
        return trianglesCount_;
    };

    virtual ~Polytope() = default;

protected:
    long long trianglesCount_{};
    std::vector<point3f> vertecies_;
    std::vector<unsigned int> indices_;
};

struct Box final : Polytope {
    Box();
};

struct Sphere final : Polytope {
    Sphere();
};

}  // namespace tire
