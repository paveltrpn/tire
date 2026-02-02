
#pragma once

#include "../subgraph.h"

namespace tired {

struct ObstaclesSubgraph final : Subgraph {
    ObstaclesSubgraph( QObject *parent = nullptr );

    auto initPipeline() -> void override;

private:
};

}  // namespace tired
