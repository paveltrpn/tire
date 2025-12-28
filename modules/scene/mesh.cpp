
module;

#include <vector>
#include <string>

#include "geometry/bounding_volumes.h"

export module scene:mesh;

import algebra;

namespace tire {

export struct TriangleIndices {
    int vertexIndex[3];
    int normalIndex[3];
    int texCoordIndex[3];
};

export struct Mesh final {
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

    [[nodiscard]] auto indicesData() const -> const TriangleIndices * {
        //
        return triangles_.data();
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

    [[nodiscard]] auto indices() const -> const std::vector<TriangleIndices> & {
        //
        return triangles_;
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

    [[nodiscard]] auto name() const -> const std::string & {
        //
        return name_;
    }

    auto setName( std::string_view name ) -> void {
        //
        name_ = name;
    }

    auto setVertices( std::vector<vector3<value_type>> vertices ) -> void {
        //
        vertices_ = std::move( vertices );
    }

    auto setTriangles( std::vector<TriangleIndices> triangles ) -> void {
        //
        triangles_ = std::move( triangles );
    }

    auto setNormals( std::vector<vector3<value_type>> normals ) -> void {
        //
        normals_ = std::move( normals );
    }

    auto setTexCoords( std::vector<vector2<value_type>> texCoords ) -> void {
        //
        texcrds_ = std::move( texCoords );
    }

    auto setVertexColors( std::vector<vector3<value_type>> vertexColors ) -> void {
        //
        vertclr_ = std::move( vertexColors );
    }

    [[nodiscard]] auto triangleCount() const -> size_t {
        //
        return triangles_.size();
    }

    [[nodiscard]] auto vertexCount() const -> size_t {
        //
        return vertices_.size();
    }

private:
    std::string name_{ "default_mesh" };
    long long trianglesCount_{};

    std::vector<vector3<value_type>> vertices_;
    std::vector<vector3<value_type>> normals_;
    std::vector<TriangleIndices> triangles_;
    std::vector<vector2<float>> texcrds_;
    std::vector<vector3<float>> vertclr_;

    AABoundingBox<value_type> bounding_;
};

}  // namespace tire
