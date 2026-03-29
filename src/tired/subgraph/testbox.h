
#pragma once

#include <vsg/all.h>

#include "subgraph.h"

namespace tired {

struct Testbox final : Subgraph {
    Testbox( vsg::Viewer* viewer );

    auto initPipeline() -> void;
    auto initDrawCommand() -> void;
};

}  // namespace tired
