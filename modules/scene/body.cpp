
#include <vector>
#include <initializer_list>
#include <iterator>

#include "body.h"
#include "geometry/polytope.h"
#include "geometry/point.h"
#include "geometry/normal.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_NODE_CPP{ false };

#include "algebra/vector3.h"
#include "algebra/matrix3.h"
#include "algebra/matrix4.h"

namespace tire {

void Body::setShapeData( std::shared_ptr<PolytopeData> data ) {
    shapeData_ = std::move( data );
    localVertecies_.reserve( shapeData_->verteciesCount() );
    localNormals_.reserve( shapeData_->verteciesCount() );
}

size_t Body::verteciesCount() const {
    return shapeData_->verteciesCount();
}

size_t Body::verteciesArraySize() const {
    return verteciesCount() * 3 * sizeof( float );
}

size_t Body::normalsArraySize() const {
    return verteciesCount() * 3 * sizeof( float );
};

size_t Body::texcrdsArraySize() const {
    return verteciesCount() * 2 * sizeof( float );
};

const point3f *Body::verteciesData() {
    return localVertecies_.data();
}

const normalf *Body::normalsData() {
    return localNormals_.data();
}

const algebra::vector2f *Body::texcrdsData() {
    return shapeData_->texcrdsData();
}

void Body::setPosition( vector3<float_type> value ) {
    position_ = value;
}

void Body::setOrientation( vector3<float_type> value ) {
    orientation_ = value;
}

void Body::setScale( vector3<float_type> value ) {
    scale_ = value;
}

void Body::setVelocity( vector3<float_type> value ) {
    velocity_ = value;
}

void Body::setTorque( vector3<float_type> value ) {
    torque_ = value;
}

void Body::setAlbedoTextureImage( const std::string &file ) {
    albedoTextureImage_ = std::make_unique<Tga>( file );
}

std::pair<int, int> Body::albedoTextureSize() {
    return { albedoTextureImage_->widht(), albedoTextureImage_->height() };
}

const uint8_t *Body::albedoTextureData() {
    return albedoTextureImage_->data();
}

void Body::applyTransormations() {
    // Update body spatial parameters
    orientation_ += torque_;
    position_ += velocity_;

    // Make transformation matrix
    const auto scale = algebra::scale( scale_.x(), scale_.y(), scale_.z() );
    const auto rotation =
        algebra::rotate( orientation_.x(), orientation_.y(), orientation_.z() );
    const auto offset =
        algebra::translate( position_.x(), position_.y(), position_.z() );

    algebra::matrix4f transform{};
    transform.multiply( offset );
    transform.multiply( rotation );
    transform.multiply( scale );

    // Apply transformation matrix to default geometry data,
    // and copy this data into local buffers.
    for ( auto i = 0; i < shapeData_->verteciesCount(); ++i ) {
        auto vertex = shapeData_->vertecies()[i];
        localVertecies_[i] = vertex.transform( transform );
        auto normal = shapeData_->normals()[i];
        localNormals_[i] = normal.transform( rotation );
    }
}

}  // namespace tire
