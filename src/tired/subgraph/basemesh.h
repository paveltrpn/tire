
#pragma once

#include "subgraph.h"

namespace tired {

struct BasemeshSubgraph final : Subgraph {
    Q_OBJECT
public:
    BasemeshSubgraph( vsg::Viewer* viewer, QObject* parent = nullptr );

    auto initPipeline() -> void override;

    Q_INVOKABLE void addExBox( float px, float py, float pz, float rx, float ry, float rz, float sx, float sy,
                               float sz );

private:
};

}  // namespace tired
