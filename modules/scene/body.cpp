
#include <vector>
#include <initializer_list>
#include <iterator>

#include "body.h"
#include "geometry/polytope.h"
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

const vector3f *Body::verteciesData() {
    return localVertecies_.data();
}

const vector3f *Body::normalsData() {
    return localNormals_.data();
}

const algebra::vector2f *Body::texcrdsData() {
    return shapeData_->texcrdsData();
}

void Body::setPosition( const vector3<float_type> &value ) {
    position_ = value;
}

void Body::setOrientation( const vector3<float_type> &value ) {
    orientation_ = value;
}

void Body::setScale( const vector3<float_type> &value ) {
    scale_ = value;
}

void Body::setVelocity( const vector3<float_type> &value ) {
    velocity_ = value;
}

void Body::setTorque( const vector3<float_type> &value ) {
    torque_ = value;
}

void Body::setMaterialName( const std::string &value ) {
    materialName_ = value;
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

void Body::setNormalmapTextureImage( const std::string &file ) {
    normalmapTextureImage_ = std::make_unique<Tga>( file );
}

std::pair<int, int> Body::normalmapTextureSize() {
    return { normalmapTextureImage_->widht(),
             normalmapTextureImage_->height() };
}

const uint8_t *Body::normalmapTextureData() {
    return normalmapTextureImage_->data();
}

void Body::applyTransormations( float duration ) {
    // Update body spatial parameters
    orientation_ += torque_.scale( duration );
    position_ += velocity_.scale( duration );

    // Make transformation matrix
    const auto scale = algebra::scale( scale_.x(), scale_.y(), scale_.z() );

    //
    const auto rotation =
        algebra::rotate( orientation_.x(), orientation_.y(), orientation_.z() );
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

}  // namespace tire
