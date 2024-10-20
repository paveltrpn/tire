
#include <vector>
#include <initializer_list>
#include <iterator>

#include "node.h"
#include "polytope.h"
#include "point.h"
#include "normal.h"

import toy_std;

using namespace toy::algebra;

namespace tire
{
Node::Node( const Polytope &body ) {
    vertecies_ = body.getVertecies();
    indices_ = body.getIndices();
}

size_t Node::getVerteciesCount() const {
    return vertecies_.size();
}

size_t Node::getVerteciesArraySize() const {
    return getVerteciesCount() * 3 * sizeof( double );
}

point3d *Node::getVerteciesData() {
    return vertecies_.data();
}

long long *Node::getIndeciessData() {
    return indices_.data();
}

void Node::setOffset( vector3d offst ) {
    offset_ = offst;
    dirty_ = true;
}

void Node::setRotate( matrix3d rtn ) {
    rotation_ = rtn;
    dirty_ = true;
}

void Node::setScale( matrix3d scl ) {
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

} // namespace tire
