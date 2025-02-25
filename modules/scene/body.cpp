
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

Body::Body( std::shared_ptr<PolytopeData> body ) {
    shapeData_ = std::move( body );
    localVertecies_.reserve( shapeData_->verteciesCount() );
    localNormals_.reserve( shapeData_->verteciesCount() );
}

std::shared_ptr<const Body> Body::asSharedPtr() const {
    return shared_from_this();
}

std::shared_ptr<Body> Body::asSharedPtr() {
    return shared_from_this();
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

void Body::setPivotOffset( algebra::vector3f offst ) {
    offset_ = algebra::translate( offst.x(), offst.y(), offst.z() );
}

void Body::setPivotRotation( algebra::vector3f rtn ) {
    rotation_ = algebra::rotate( rtn.x(), rtn.y(), rtn.z() );
}

void Body::setPivotScale( algebra::vector3f scl ) {
    scale_ = algebra::scale( scl.x(), scl.y(), scl.z() );
}

void Body::setMomentum( algebra::vector3f rtn ) {
    momentum_ = algebra::rotate( rtn.x(), rtn.y(), rtn.z() );
}

void Body::setTextureImage( const std::string &file ) {
    textureImage_ = std::make_unique<Tga>( file );
}

std::pair<int, int> Body::textureSize() {
    return { textureImage_->widht(), textureImage_->height() };
}

const uint8_t *Body::textureData() {
    return textureImage_->data();
}

void Body::applyTransormations() {
    if ( useMomentum_ ) {
        rotation_.multiply( momentum_ );
    }

    // const auto transform = offset_ * totalRotation * scale_;+
    algebra::matrix4f transform{};
    transform.multiply( offset_ );
    transform.multiply( rotation_ );
    transform.multiply( scale_ );
    const auto vertecies = shapeData_->vertecies();
    const auto normals = shapeData_->normals();
    for ( auto i = 0; i < vertecies.size(); ++i ) {
        localVertecies_[i] = vertecies[i].transform( transform );
        localNormals_[i] = normals[i].transform( rotation_ );
    }
}

}  // namespace tire
