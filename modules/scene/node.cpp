
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

Node::Node( PolytopeData *body ) {
    shapeData_ = body;
    localVertecies_.reserve( shapeData_->verteciesCount() );
    localIndecies = shapeData_->indices();
    // log::debug<DEBUG_OUTPUT_NODE_CPP>( "appended vertecies: {}, indecies: {}",
    //                                    vertecies_.size(), indices_.size() );
}

std::shared_ptr<const Node> Node::asSharedPtr() const {
    return shared_from_this();
}

std::shared_ptr<Node> Node::asSharedPtr() {
    return shared_from_this();
}

size_t Node::verteciesCount() const {
    return shapeData_->verteciesCount();
}

size_t Node::verteciesArraySize() const {
    return verteciesCount() * 3 * sizeof( float );
}

size_t Node::indeciesCount() const {
    return shapeData_->indiciesCount();
}

size_t Node::indeciesArraySize() const {
    return indeciesCount() * sizeof( unsigned int );
}

const point3f *Node::verteciesData() {
    return localVertecies_.data();
}

const unsigned int *Node::indeciesData() {
    return localIndecies.data();
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
    algebra::matrix4f totalRotation{};
    if ( useMomentum_ ) {
        totalRotation = momentum_ * rotation_;
    } else {
        totalRotation = rotation_;
    }
    const auto transform = offset_ * totalRotation * scale_;

    const auto vertecies = shapeData_->vertecies();
    for ( auto i = 0; i < vertecies.size(); ++i ) {
        localVertecies_[i] = vertecies[i];
        localVertecies_[i].transform( transform );
    }
}

}  // namespace tire
