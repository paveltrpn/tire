
#include "node_list.h"

namespace tire {

void NodeList::push_back( const std::shared_ptr<Node> &node ) {
    list_.push_back( node );
}

void NodeList::push_back( Node &node ) {
    list_.push_back( node.asSharedPtr() );
}

}  // namespace tire
