
#pragma once

#include "../subgraph.h"

namespace tired {

struct BasemeshSubgraph final : Subgraph {
    Q_OBJECT
public:
    BasemeshSubgraph( QObject *parent = nullptr );

    auto initPipeline() -> void override;

    auto addChild( vsg::ref_ptr<vsg::Node> node ) -> void;

    Q_INVOKABLE void addExBox( float px, float py, float pz, float rx, float ry, float rz, float sx, float sy,
                               float sz );

private:
    vsg::ref_ptr<vsg::MatrixTransform> baseNode_{};
};

}  // namespace tired
