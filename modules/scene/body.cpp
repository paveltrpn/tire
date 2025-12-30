
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

    Body( std::shared_ptr<SeparatedBuffersMesh> mesh )
        : mesh_{ std::move( mesh ) } {
        // Resize buffer to fit mesh size.
        buffer_.vertices_.resize( mesh_->verteciesCount() );
        buffer_.normals_.resize( mesh_->verteciesCount() );
    }

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

    [[nodiscard]]
    auto verteciesArraySize() const -> size_t {
        return verteciesCount() * 3 * sizeof( value_type );
    }

    [[nodiscard]]
    size_t normalsArraySize() const {
        return verteciesCount() * 3 * sizeof( value_type );
    };

    [[nodiscard]]
    size_t texcrdsArraySize() const {
        return verteciesCount() * 2 * sizeof( value_type );
    };

    [[nodiscard]]
    auto verteciesData() const -> const vector3<value_type> * {
        return buffer_.vertices_.data();
    }

    [[nodiscard]]
    auto normalsData() const -> const vector3<value_type> * {
        return buffer_.normals_.data();
    }

    [[nodiscard]]
    const algebra::vector2f *texcrdsData() {
        return mesh_->texcrds_.data();
    }

    void setBounding( AABoundingBox value ) {
        //
        bounding_ = value;
    }

    [[nodiscard]]
    auto bounding() const -> AABoundingBox {
        return bounding_;
    };

    void setAlbedoColor( const std::string &name ) {
        //
        albedoColor_ = Colorf{ name };
    }

    [[nodiscard]]
    auto albedoColor() const -> Colorf {
        return albedoColor_;
    };

    void setPosition( const vector3<value_type> &value ) {
        //
        position_ = value;
    }

    void setOrientation( const vector3<value_type> &value ) {
        //
        orientation_ = value;
    }

    void setScale( const vector3<value_type> &value ) {
        //
        scale_ = value;
    }

    void setVelocity( const vector3<value_type> &value ) {
        //
        velocity_ = value;
    }

    void setTorque( const vector3<value_type> &value ) {
        //
        torque_ = value;
    }

    void setMaterialName( const std::string &value ) {
        //
        materialName_ = value;
    }

    [[nodiscard]]
    vector3<value_type> position() const {
        return position_;
    }

    [[nodiscard]]
    vector3<value_type> orientation() const {
        return orientation_;
    }

    [[nodiscard]]
    vector3<value_type> scale() const {
        return scale_;
    }

    [[nodiscard]]
    vector3<value_type> velocity() const {
        return velocity_;
    }

    [[nodiscard]]
    vector3<value_type> torque() const {
        return torque_;
    }

    [[nodiscard]]
    std::string materialName() const {
        return materialName_;
    };

    // Calculate transformation matrix, apply this
    // matrix to default geometry data and copy this data
    // into local buffers
    void applyTransormations( float duration ) {
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
