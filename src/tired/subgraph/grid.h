
#pragma once

#include <vsg/all.h>

#include "subgraph.h"

namespace tired {

struct Grid final : Subgraph {
    Grid(vsg::Viewer* viewer);

    auto initPipeline() -> void;

private:
};

}  // namespace tired
