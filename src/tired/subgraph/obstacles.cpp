
#include "obstacles.h"

namespace tired {

ObstaclesSubgraph::ObstaclesSubgraph( vsg::Viewer* viewer, QObject* parent )
    : Subgraph{ viewer, parent } {
}

auto ObstaclesSubgraph::initPipeline() -> void {
}

}  // namespace tired
