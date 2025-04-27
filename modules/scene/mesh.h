
#pragma once

#include <vector>

#include "algebra/vector2.h"
#include "algebra/vector3.h"
#include "geometry/bounding_volumes.h"

namespace tire {

struct Mesh final {
    using value_type = float;

    Mesh( const std::string& path );

    [[nodiscard]] size_t verteciesCount() const { return vertecies_.size(); };

    [[nodiscard]] const vector3<value_type>* verteciesData() const {
        return vertecies_.data();
    };

    [[nodiscard]] const vector3<value_type>* normalsData() const {
        return normals_.data();
    }

    [[nodiscard]] const vector2<value_type>* texcrdsData() const {
        return texcrds_.data();
    }

    [[nodiscard]] const std::vector<vector3<value_type>>& vertecies() const {
        return vertecies_;
    };

    [[nodiscard]] const std::vector<vector3<value_type>>& normals() const {
        return normals_;
    }

    [[nodiscard]] long long trianglesCount() { return trianglesCount_; }

    [[nodiscard]] AABoundingBox<value_type> bounding() { return bounding_; }

private:
    long long trianglesCount_{};
    AABoundingBox<value_type> bounding_;
    std::vector<vector3<value_type>> vertecies_;
    std::vector<vector3<value_type>> normals_;
    std::vector<vector2<value_type>> texcrds_;
};

}  // namespace tire