
module;

#include <vector>
#include <memory>

export module scene:body;

import algebra;
import geometry;
import image;

import :mesh;
import :material;

namespace tire {

using namespace algebra;

struct Body final {
    using value_type = SeparatedBuffersMesh::value_type;

    Body() = default;

    explicit Body( std::shared_ptr<SeparatedBuffersMesh> mesh )
        : mesh_{ std::move( mesh ) } {
        // Resize buffer to fit mesh size.
        buffer_.vertices_.resize( mesh_->verteciesCount() );
        buffer_.normals_.resize( mesh_->verteciesCount() );
    }

    Body( const Body &other ) = default;
    Body( Body &&other ) = default;

    auto operator=( const Body &other ) -> Body & = default;
    auto operator=( Body &&other ) -> Body & = default;

    ~Body() = default;

    [[nodiscard]]
    auto verteciesCount() const -> size_t {
        //
        return mesh_->verteciesCount();
    }

    [[nodiscard]] auto bufferVerticesSize() const -> size_t {
        //
        return verteciesCount() * 3 * sizeof( value_type );
    }

    [[nodiscard]] auto bufferNormalsSize() const -> size_t {
        //
        return verteciesCount() * 3 * sizeof( value_type );
    }

    [[nodiscard]] auto bufferTexcrdsSize() const -> size_t {
        //
        return verteciesCount() * 2 * sizeof( value_type );
    }

    [[nodiscard]] auto bufferVertclrsSize() const -> size_t {
        //
        return verteciesCount() * 4 * sizeof( value_type );
    }

    [[nodiscard]]
    auto verteciesData() const -> const vector3<value_type> * {
        return buffer_.vertices_.data();
    }

    [[nodiscard]]
    auto normalsData() const -> const vector3<value_type> * {
        return buffer_.normals_.data();
    }

    [[nodiscard]]
    auto texcrdsData() const -> const algebra::vector2<float> * {
        return mesh_->texcrds_.data();
    }

    [[nodiscard]]
    auto bounding() const -> AABoundingBox {
        return mesh_->bounding_;
    };

    auto setAlbedoColor( const std::string &name ) -> void {
        //
        albedoColor_ = Colorf{ name };
    }

    [[nodiscard]]
    auto albedoColor() const -> Colorf {
        return albedoColor_;
    };

    auto setPosition( const vector3<value_type> &value ) -> void {
        //
        position_ = value;
    }

    auto setOrientation( const vector3<value_type> &value ) -> void {
        //
        orientation_ = value;
    }

    auto setScale( const vector3<value_type> &value ) -> void {
        //
        scale_ = value;
    }

    auto setVelocity( const vector3<value_type> &value ) -> void {
        //
        velocity_ = value;
    }

    auto setTorque( const vector3<value_type> &value ) -> void {
        //
        torque_ = value;
    }

    auto setMaterialName( const std::string &value ) -> void {
        //
        materialName_ = value;
    }

    [[nodiscard]]
    auto position() const -> vector3<value_type> {
        return position_;
    }

    [[nodiscard]]
    auto orientation() const -> vector3<value_type> {
        return orientation_;
    }

    [[nodiscard]]
    auto scale() const -> vector3<value_type> {
        return scale_;
    }

    [[nodiscard]]
    auto velocity() const -> vector3<value_type> {
        return velocity_;
    }

    [[nodiscard]]
    auto torque() const -> vector3<value_type> {
        return torque_;
    }

    [[nodiscard]]
    auto materialName() const -> std::string {
        return materialName_;
    };

    // Calculate transformation matrix, apply this
    // matrix to default geometry data and copy this data
    // into local buffers
    auto applyTransformations( float duration ) -> void {
        // Update body spatial parameters
        orientation_ += torque_.scale( duration );
        position_ += velocity_.scale( duration );

        // Make transformation matrix
        const auto scale = algebra::scale( scale_.x(), scale_.y(), scale_.z() );

        //
        const auto rotation = algebra::rotate( orientation_.x(), orientation_.y(), orientation_.z() );
        auto itRotation = rotation.inverse();
        itRotation.transpose_self();

        const auto offset = algebra::translate( position_.x(), position_.y(), position_.z() );

        // Get combined transformation matrix
        algebra::matrix4f transform{};
        transform.multiply( offset );
        transform.multiply( rotation );
        transform.multiply( scale );

        // Apply transformation matrix to default geometry data,
        // and copy this data into local buffers.
        for ( auto i = 0; i < mesh_->verteciesCount(); ++i ) {
            // Update vertex positions
            auto vertex = mesh_->vertices_[i];
            buffer_.vertices_[i] = transform.mult_vector3( vertex );

            // Update normals
            auto normal = mesh_->normals_[i];
            buffer_.normals_[i] = itRotation.mult_vector3( normal );
        }
    }

private:
    std::shared_ptr<SeparatedBuffersMesh> mesh_;

    // Buffers for local geometry data. This buffers fill
    // every frame with geometry data taken from default
    // shape data and transform according to actual spatial
    // information
    struct LocalBuffer final {
        std::vector<vector3<value_type>> vertices_;
        std::vector<vector3<value_type>> normals_;
    };
    LocalBuffer buffer_{};

    AABoundingBox bounding_{};

    // Body spatial information
    vector3<value_type> position_{};
    vector3<value_type> orientation_{};
    vector3<value_type> scale_{};
    vector3<value_type> velocity_{};
    vector3<value_type> torque_{};

    // Body material information
    Colorf albedoColor_{};
    std::string materialName_{};
};

}  // namespace tire
