
#include "node_list.h"

namespace tire {

void NodeList::push_back( const std::shared_ptr<Node> &node ) {
    list_.push_back( node );
}

}  // namespace tire
