
#pragma once

#include "../subgraph.h"

namespace tired {

struct BasemeshSubgraph final : Subgraph {
    auto initPipeline() -> void override;

    auto addChild( vsg::ref_ptr<vsg::Node> node ) -> void;

private:
    vsg::ref_ptr<vsg::MatrixTransform> baseNode_{};
};

}  // namespace tired
