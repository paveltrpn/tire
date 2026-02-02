
#pragma once

#include "../subgraph.h"

namespace tired {

struct ServiceObjectsSubgraph final : Subgraph {
    ServiceObjectsSubgraph( QObject *parent = nullptr );

    auto initPipeline() -> void override;

private:
};

}  // namespace tired
