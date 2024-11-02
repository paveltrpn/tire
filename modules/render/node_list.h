
#pragma once

#include <memory.h>
#include <list>

#include "geometry/node.h"

namespace tire {

struct NodeList final {
    NodeList( const NodeList &other ) = delete;
    NodeList( NodeList &&other ) = delete;

    NodeList &operator=( const NodeList &other ) = delete;
    NodeList &operator=( NodeList &&other ) = delete;

    NodeList() = default;

    void push_back( const std::shared_ptr<Node> &node );

private:
    std::list<std::shared_ptr<Node>> list_;
};

}  // namespace tire
