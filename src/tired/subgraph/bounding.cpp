
#include "bounding.h"

namespace tired {

Bounding::Bounding( const QObject* parent ) {
    //
}

// ===============================================================================

BoundingSubgraph::BoundingSubgraph( vsg::Viewer* viewer )
    : Subgraph{ viewer } {
    //
}

auto BoundingSubgraph::initPipeline() -> void{
    //
};

}  // namespace tired
