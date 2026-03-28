
#pragma once

#include "subgraph.h"

namespace tired {

struct ObstaclesSubgraph final : Subgraph {
    ObstaclesSubgraph( vsg::Viewer* viewer, QObject* parent = nullptr );

    auto initPipeline() -> void override;

private:
};

}  // namespace tired
