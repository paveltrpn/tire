
#include "ServiceObjectsSubgraph.h"

namespace tired {

ServiceObjectsSubgraph::ServiceObjectsSubgraph( vsg::Viewer* viewer, QObject* parent )
    : Subgraph{ viewer, parent } {
}

auto ServiceObjectsSubgraph::initPipeline() -> void {
}

}  // namespace tired
