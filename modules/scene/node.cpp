
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
    return verteciesCount() * 3 * sizeof( double );
}

size_t Node::indeciesCount() const {
    return indices_.size();
}

size_t Node::indeciesArraySize() const {
    return indeciesCount() * sizeof( unsigned int );
}

point3d *Node::verteciesData() {
    return vertecies_.data();
}

unsigned int *Node::indeciesData() {
    return indices_.data();
}

void Node::setOffset( algebra::vector3d offst ) {
    offset_ = offst;
    dirty_ = true;
}

void Node::setRotate( algebra::matrix3d rtn ) {
    rotation_ = rtn;
    dirty_ = true;
}

void Node::setScale( algebra::matrix3d scl ) {
    scale_ = scl;
    dirty_ = true;
}

void Node::applyRotate() {
    if ( dirty_ ) {
        for ( auto i = 0; i < vertecies_.size(); ++i ) {
            // vertecies_[i].transform( rotation_ );
        }
        dirty_ = false;
    }
}

void Node::applyScale() {
    if ( dirty_ ) {
        for ( auto i = 0; i < vertecies_.size(); ++i ) {
            //vertecies_[i].transform( scale_ );
        }
        dirty_ = false;
    }
}

}  // namespace tire
