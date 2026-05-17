

#pragma once

#include <vsg/all.h>

#include "subgraph.h"

namespace tired {

struct BoundingSubgraph;

struct Bounding final : public QObject {
    Q_OBJECT

public:
    Bounding( const QObject* parent = nullptr );
};

// ===============================================================================

struct BoundingSubgraph final : public Subgraph {
    BoundingSubgraph( vsg::Viewer* viewer );

    auto initPipeline() -> void override;

private:
};

}  // namespace tired