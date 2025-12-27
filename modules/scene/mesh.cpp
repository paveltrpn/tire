
module;

#include <vector>
#include <string>
#include <fstream>
#include <format>

#include "geometry/bounding_volumes.h"

export module scene:mesh;

import algebra;

namespace tire {

struct Mesh final {
    using value_type = float;

    Mesh() = default;
    Mesh( const Mesh &other ) = default;
    Mesh( Mesh &&other ) = default;

    auto operator=( const Mesh &other ) -> Mesh & = default;
    auto operator=( Mesh &&other ) -> Mesh & = default;

    ~Mesh() = default;

    [[nodiscard]] auto verticesData() const -> const vector3<value_type> * {
        //
        return vertices_.data();
    };

    [[nodiscard]] auto normalsData() const -> const vector3<value_type> * {
        //
        return normals_.data();
    }

    [[nodiscard]] auto texcrdsData() const -> const vector2<value_type> * {
        //
        return texcrds_.data();
    }

    [[nodiscard]] auto vertclrData() const -> const vector3<value_type> * {
        //
        return vertclr_.data();
    }

    [[nodiscard]] auto vertecies() const -> const std::vector<vector3<value_type>> & {
        //
        return vertices_;
    };

    [[nodiscard]] auto normals() const -> const std::vector<vector3<value_type>> & {
        //
        return normals_;
    }

    [[nodiscard]] auto texcrds() const -> const std::vector<vector2<value_type>> & {
        //
        return texcrds_;
    }

    [[nodiscard]] auto vertclr() const -> const std::vector<vector3<value_type>> & {
        //
        return vertclr_;
    }

    [[nodiscard]] auto trianglesCount() const -> long long {
        //
        return trianglesCount_;
    }

    [[nodiscard]] auto bounding() const -> AABoundingBox<value_type> {
        //
        return bounding_;
    }

private:
    long long trianglesCount_{};

    std::vector<vector3<value_type>> vertices_;
    std::vector<vector3<value_type>> normals_;
    std::vector<vector2<value_type>> texcrds_;
    std::vector<vector3<value_type>> vertclr_;

    AABoundingBox<value_type> bounding_;
};

}  // namespace tire
