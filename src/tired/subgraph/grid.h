
#pragma once

#include <vsg/all.h>

#include "subgraph.h"

namespace tired {

struct Grid final : Subgraph {
    Grid( vsg::Viewer* viewer );

    auto initPipeline() -> void override;
    auto initDrawCommand() -> void;

private:
    vsg::ref_ptr<vsg::PushConstants> _confPushConst{};
};

}  // namespace tired
