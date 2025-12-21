
module;

#include <vector>
#include <memory>

#include "geometry/polytope.h"
#include "geometry/bounding_volumes.h"

export module scene:body;

import algebra;
import image;

namespace tire {

using namespace algebra;

struct Body final {
    using value_type = float;

    Body() = default;

    void setShapeData( std::shared_ptr<PolytopeData> data ) {
        shapeData_ = std::move( data );
        localVertecies_.resize( shapeData_->verteciesCount() );
        localNormals_.resize( shapeData_->verteciesCount() );
    }

    size_t verteciesCount() const { return shapeData_->verteciesCount(); }

    size_t verteciesArraySize() const {
        return verteciesCount() * 3 * sizeof( float );
    }

    size_t normalsArraySize() const {
        return verteciesCount() * 3 * sizeof( float );
    };

    size_t texcrdsArraySize() const {
        return verteciesCount() * 2 * sizeof( float );
    };

    const vector3f* verteciesData() { return localVertecies_.data(); }

    const vector3f* normalsData() { return localNormals_.data(); }

    const algebra::vector2f* texcrdsData() { return shapeData_->texcrdsData(); }

    void setBounding( BoundingVolume<float> value ) { bounding_ = value; }
    const BoundingVolume<float> bounding() { return bounding_; };

    void setAlbedoColor( const std::string& name ) {
        albedoColor_ = Colorf{ name };
    }

    [[nodiscard]]
    Colorf albedoColor() const {
        return albedoColor_;
    };

    void setPosition( const vector3<value_type>& value ) { position_ = value; }

    void setOrientation( const vector3<value_type>& value ) {
        orientation_ = value;
    }

    void setScale( const vector3<value_type>& value ) { scale_ = value; }

    void setVelocity( const vector3<value_type>& value ) { velocity_ = value; }

    void setTorque( const vector3<value_type>& value ) { torque_ = value; }

    void setMaterialName( const std::string& value ) { materialName_ = value; }

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
        const auto rotation = algebra::rotate(
            orientation_.x(), orientation_.y(), orientation_.z() );
        auto itRotation = rotation.inverse();
        itRotation.transposeSelf();

        const auto offset =
            algebra::translate( position_.x(), position_.y(), position_.z() );

        // Get combined transformation matrix
        algebra::matrix4f transform{};
        transform.multiply( offset );
        transform.multiply( rotation );
        transform.multiply( scale );

        // Apply transformation matrix to default geometry data,
        // and copy this data into local buffers.
        for ( auto i = 0; i < shapeData_->verteciesCount(); ++i ) {
            // Update vertex positions
            auto vertex = shapeData_->vertecies()[i];
            localVertecies_[i] = transform.mult_vector3( vertex );

            // Update normals
            auto normal = shapeData_->normals()[i];
            localNormals_[i] = itRotation.mult_vector3( normal );
        }
    }

private:
    // Default body geometry data.
    std::shared_ptr<PolytopeData> shapeData_{};

    // Buffers for local geometry data. This buffers fill
    // every frame with geometry data taken from default
    // shape data and transform according to actual spatial
    // information
    std::vector<vector3f> localVertecies_{};
    std::vector<vector3f> localNormals_{};

    BoundingVolume<value_type> bounding_{};

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
