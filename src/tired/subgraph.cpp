
#include "subgraph.h"

namespace tired {

Subgraph::Subgraph()
    : QObject{}
    , stateGroup_{ vsg::StateGroup::create() } {
}

auto Subgraph::stateGroup() -> vsg::ref_ptr<vsg::StateGroup> {
    return stateGroup_;
}

}  // namespace tired
