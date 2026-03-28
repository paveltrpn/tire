
#pragma once

#include "subgraph.h"

namespace tired {

struct ObstaclesSubgraph final : Subgraph {
    ObstaclesSubgraph( vsg::Viewer* viewer );

    auto initPipeline() -> void override;

private:
};

}  // namespace tired
