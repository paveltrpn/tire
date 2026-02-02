
#pragma once

#include "../subgraph.h"

namespace tired {

struct ServiceObjectsSubgraph final : Subgraph {
    auto initPipeline() -> void override;

private:
};

}  // namespace tired
