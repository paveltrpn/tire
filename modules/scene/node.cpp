
#include <vector>
#include <initializer_list>
#include <iterator>

#include "node.h"
#include "geometry/polytope.h"
#include "geometry/point.h"
#include "geometry/normal.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_NODE_CPP{ false };

#include "algebra/vector3.h"
#include "algebra/matrix3.h"
#include "algebra/matrix4.h"

namespace tire {

Node::Node( const Polytope &body ) {
    vertecies_ = body.getVertecies();
    indices_ = body.getIndices();
    trianglesCount_ = body.getTrianglesCount();

    log::debug<DEBUG_OUTPUT_NODE_CPP>( "appended vertecies: {}, indecies: {}",
                                       vertecies_.size(), indices_.size() );
}

std::shared_ptr<const Node> Node::asSharedPtr() const {
    return shared_from_this();
}

std::shared_ptr<Node> Node::asSharedPtr() {
    return shared_from_this();
}

size_t Node::verteciesCount() const {
    return vertecies_.size();
}

size_t Node::verteciesArraySize() const {
    return verteciesCount() * 3 * sizeof( float );
}

size_t Node::indeciesCount() const {
    return indices_.size();
}

size_t Node::indeciesArraySize() const {
    return indeciesCount() * sizeof( unsigned int );
}

point3f *Node::verteciesData() {
    return vertecies_.data();
}

unsigned int *Node::indeciesData() {
    return indices_.data();
}

void Node::setPivotOffset( algebra::vector3f offst ) {
    offset_ = algebra::translate( offst.x(), offst.y(), offst.z() );
}

void Node::setPivotRotation( algebra::vector3f rtn ) {
    rotation_ = algebra::rotate( rtn.x(), rtn.y(), rtn.z() );
}

void Node::setPivotScale( algebra::vector3f scl ) {
    scale_ = algebra::scale( scl.x(), scl.y(), scl.z() );
}

void Node::setMomentum( algebra::vector3f rtn ) {
    momentum_ = algebra::rotate( rtn.x(), rtn.y(), rtn.z() );
}

void Node::applyPivotTransormations() {
    algebra::matrix4f totalRotation;
    if ( useMomentum_ ) {
        totalRotation = rotation_ * momentum_;
    } else {
        totalRotation = rotation_;
    }
    const auto transform = offset_ * totalRotation * scale_;
    for ( auto i = 0; i < vertecies_.size(); ++i ) {
        vertecies_[i].transform( transform );
    }
}

void Node::applyInversePivotTransormations() {
    const auto transform = offset_ * rotation_ * scale_;
    const auto inverseTransform = transform.inverse();
    for ( auto i = 0; i < vertecies_.size(); ++i ) {
        vertecies_[i].transform( inverseTransform );
    }
}

}  // namespace tire
