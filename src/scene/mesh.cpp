
module;

#include <vector>
#include <string>

export module scene:mesh;

import algebra;
import geometry;

namespace tire {

using namespace algebra;

// Gloabal mesh vertex value type
using MeshValueType = float;

// Mesh interface.
struct Mesh {
    using value_type = MeshValueType;

    Mesh() = default;
    Mesh( const Mesh &other ) = default;
    Mesh( Mesh &&other ) = default;

    auto operator=( const Mesh &other ) -> Mesh & = default;
    auto operator=( Mesh &&other ) -> Mesh & = default;

    virtual ~Mesh() = default;

    [[nodiscard]] virtual auto verteciesCount() const -> size_t = 0;
    [[nodiscard]] virtual auto tringlesCount() const -> size_t = 0;
};

// Dual-Indexing (Per-Face Normals/UVs).
// Compatible with .obj, FBX, glTF
struct ObjTriangleIndices {
    int vertexIndex[3];
    int normalIndex[3];
    int texCoordIndex[3];
};

struct ObjMesh final : Mesh {
    using value_type = MeshValueType;

    ObjMesh() = default;
    ObjMesh( const ObjMesh &other ) = default;
    ObjMesh( ObjMesh &&other ) = default;

    auto operator=( const ObjMesh &other ) -> ObjMesh & = default;
    auto operator=( ObjMesh &&other ) -> ObjMesh & = default;

    ~ObjMesh() override = default;

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

    [[nodiscard]] auto bounding() const -> AABoundingBox {
        //
        return bounding_;
    }

    auto setBounding( const AABoundingBox &bounding ) -> void {
        //
        bounding_ = bounding;
    }

    [[nodiscard]] auto name() const -> const std::string & {
        //
        return name_;
    }

    auto setName( const std::string &name ) -> void {
        //
        name_ = name;
    }

    [[nodiscard]] auto verteciesCount() const -> size_t override {
        //
        return triangles_.size() * 3;
    };

    [[nodiscard]] auto tringlesCount() const -> size_t override {
        //
        return triangles_.size();
    };

    std::vector<vector3<value_type>> vertices_{};
    std::vector<vector3<value_type>> normals_{};
    std::vector<vector2<float>> texcrds_{};

    std::vector<vector3<float>> vertclr_{};

    std::vector<ObjTriangleIndices> triangles_{};

    std::string name_{};
    AABoundingBox bounding_{};
};

// ====================================================================

export struct InterleavedMesh final : Mesh {
    using value_type = MeshValueType;

    struct Vertex {
        vector3<value_type> pos;
        vector3<value_type> norm;
        vector2<float> uv;
    };

    InterleavedMesh() = default;
    InterleavedMesh( const InterleavedMesh &other ) = default;
    InterleavedMesh( InterleavedMesh &&other ) = default;

    explicit InterleavedMesh( const ObjMesh &other )
        : name_{ other.name_ }
        , bounding_{ other.bounding_ } {
        for ( auto &&indecies : other.triangles_ ) {
            const auto v1 = other.vertices_[indecies.vertexIndex[0]];
            const auto v2 = other.vertices_[indecies.vertexIndex[1]];
            const auto v3 = other.vertices_[indecies.vertexIndex[2]];

            const auto n1 = other.normals_[indecies.normalIndex[0]];
            const auto n2 = other.normals_[indecies.normalIndex[1]];
            const auto n3 = other.normals_[indecies.normalIndex[2]];

            const auto tx1 = other.texcrds_[indecies.texCoordIndex[0]];
            const auto tx2 = other.texcrds_[indecies.texCoordIndex[1]];
            const auto tx3 = other.texcrds_[indecies.texCoordIndex[2]];

            auto vertex1 = Vertex{ v1, n1, tx1 };
            auto vertex2 = Vertex{ v2, n2, tx2 };
            auto vertex3 = Vertex{ v3, n3, tx3 };

            vertices_.push_back( vertex1 );
            vertices_.push_back( vertex2 );
            vertices_.push_back( vertex3 );
        };
    }

    auto operator=( const InterleavedMesh &other ) -> InterleavedMesh & = default;
    auto operator=( InterleavedMesh &&other ) -> InterleavedMesh & = default;

    ~InterleavedMesh() override = default;

    [[nodiscard]] auto bounding() const -> AABoundingBox {
        //
        return bounding_;
    }

    auto setBounding( const AABoundingBox &bounding ) -> void {
        //
        bounding_ = bounding;
    }

    [[nodiscard]] auto name() const -> const std::string & {
        //
        return name_;
    }

    auto setName( const std::string &name ) -> void {
        //
        name_ = name;
    }

    [[nodiscard]] auto verteciesCount() const -> size_t override {
        //
        return vertices_.size();
    };

    [[nodiscard]] auto tringlesCount() const -> size_t override {
        //
        return vertices_.size() / 3;
    };

    std::vector<Vertex> vertices_;

    std::string name_{};
    AABoundingBox bounding_;
};

// ====================================================================

export struct SeparatedBuffersMesh final : Mesh {
    using value_type = MeshValueType;

    SeparatedBuffersMesh() = default;
    SeparatedBuffersMesh( const SeparatedBuffersMesh &other ) = default;
    SeparatedBuffersMesh( SeparatedBuffersMesh &&other ) = default;

    explicit SeparatedBuffersMesh( const ObjMesh &other )
        : name_{ other.name_ }
        , bounding_{ other.bounding_ } {
        for ( auto &&indecies : other.triangles_ ) {
            const auto v1 = other.vertices_[indecies.vertexIndex[0]];
            const auto v2 = other.vertices_[indecies.vertexIndex[1]];
            const auto v3 = other.vertices_[indecies.vertexIndex[2]];

            const auto n1 = other.normals_[indecies.normalIndex[0]];
            const auto n2 = other.normals_[indecies.normalIndex[1]];
            const auto n3 = other.normals_[indecies.normalIndex[2]];

            const auto tx1 = other.texcrds_[indecies.texCoordIndex[0]];
            const auto tx2 = other.texcrds_[indecies.texCoordIndex[1]];
            const auto tx3 = other.texcrds_[indecies.texCoordIndex[2]];

            vertices_.push_back( v1 );
            vertices_.push_back( v2 );
            vertices_.push_back( v3 );

            normals_.push_back( n1 );
            normals_.push_back( n2 );
            normals_.push_back( n3 );

            texcrds_.push_back( tx1 );
            texcrds_.push_back( tx2 );
            texcrds_.push_back( tx3 );
        };
    }

    auto operator=( const SeparatedBuffersMesh &other ) -> SeparatedBuffersMesh & = default;
    auto operator=( SeparatedBuffersMesh &&other ) -> SeparatedBuffersMesh & = default;

    ~SeparatedBuffersMesh() override = default;

    [[nodiscard]] auto bounding() const -> AABoundingBox {
        //
        return bounding_;
    }

    auto setBounding( const AABoundingBox &bounding ) -> void {
        //
        bounding_ = bounding;
    }

    [[nodiscard]] auto name() const -> const std::string & {
        //
        return name_;
    }

    auto setName( const std::string &name ) -> void {
        //
        name_ = name;
    }

    [[nodiscard]] auto verteciesCount() const -> size_t override {
        //
        return vertices_.size();
    };

    [[nodiscard]] auto tringlesCount() const -> size_t override {
        //
        return vertices_.size() / 3;
    };

    std::vector<vector3<value_type>> vertices_;
    std::vector<vector3<value_type>> normals_;
    std::vector<vector2<float>> texcrds_;

    std::string name_{};
    AABoundingBox bounding_;
};

}  // namespace tire
