
#pragma once

#include "subgraph.h"

namespace tired {

struct BasemeshSubgraph final : Subgraph {
    BasemeshSubgraph( vsg::Viewer* viewer );

    auto initPipeline() -> void override;

private:
};

}  // namespace tired
