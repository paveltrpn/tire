#pragma once

#include <vsg/all.h>

#include "subgraph.h"

namespace tired {

struct MarkerSubgraph final : Subgraph {
    MarkerSubgraph( vsg::Viewer* viewer );

    auto initPipeline() -> void override;

private:
};

}
