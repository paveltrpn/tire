
#pragma once

#include "subgraph.h"

namespace tired {

struct SceneObjectSubgraph final : Subgraph {
    SceneObjectSubgraph( vsg::Viewer* viewer );

    auto initPipeline() -> void override;

private:
};

}  // namespace tired
