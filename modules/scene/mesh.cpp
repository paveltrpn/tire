
module;

#include <vector>
#include <string>

#include "geometry/bounding_volumes.h"

export module scene:mesh;

import algebra;

namespace tire {

// Gloabal mesh vertex value type
using MeshValueType = float;

// Dual-Indexing (Per-Face Normals/UVs).
// Compatible with .obj, FBX, glTF
struct ObjTriangleIndices {
    int vertexIndex[3];
    int normalIndex[3];
    int texCoordIndex[3];
};

struct ObjMesh final {
    using value_type = MeshValueType;

    ObjMesh() = default;
    ObjMesh( const ObjMesh &other ) = default;
    ObjMesh( ObjMesh &&other ) = default;

    auto operator=( const ObjMesh &other ) -> ObjMesh & = default;
    auto operator=( ObjMesh &&other ) -> ObjMesh & = default;

    ~ObjMesh() = default;

    [[nodiscard]] auto verticesData() const -> const vector3<value_type> * {
        //
        return vertices_.data();
    };

    [[nodiscard]] auto indicesData() const -> const ObjTriangleIndices * {
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

    [[nodiscard]] auto indices() const -> const std::vector<ObjTriangleIndices> & {
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

    [[nodiscard]] auto bounding() const -> AABoundingBox<value_type> {
        //
        return bounding_;
    }

    auto setVertices( std::vector<vector3<value_type>> vertices ) -> void {
        //
        vertices_ = std::move( vertices );
    }

    auto setTriangles( std::vector<ObjTriangleIndices> triangles ) -> void {
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

    [[nodiscard]] auto name() const -> const std::string & {
        //
        return name_;
    }

    auto setName( const std::string &name ) -> void {
        //
        name_ = name;
    }

    std::vector<vector3<value_type>> vertices_{};
    std::vector<vector3<value_type>> normals_{};
    std::vector<ObjTriangleIndices> triangles_{};
    std::vector<vector2<float>> texcrds_{};
    std::vector<vector3<float>> vertclr_{};

    std::string name_{};
    AABoundingBox<value_type> bounding_{};
};

// ====================================================================

export struct InterleavedMesh final {
    using value_type = MeshValueType;

    InterleavedMesh() = default;
    InterleavedMesh( const InterleavedMesh &other ) = default;
    InterleavedMesh( InterleavedMesh &&other ) = default;

    explicit InterleavedMesh( const ObjMesh &other ) {

    };

    auto operator=( const InterleavedMesh &other ) -> InterleavedMesh & = default;
    auto operator=( InterleavedMesh &&other ) -> InterleavedMesh & = default;

    ~InterleavedMesh() = default;

    [[nodiscard]] auto name() const -> const std::string & {
        //
        return name_;
    }

    auto setName( const std::string &name ) -> void {
        //
        name_ = name;
    }

    std::vector<vector3<value_type>> vertices_;
    std::vector<vector3<value_type>> normals_;
    std::vector<vector2<float>> texcrds_;

    std::string name_{};
    AABoundingBox<value_type> bounding_;
};

}  // namespace tire
