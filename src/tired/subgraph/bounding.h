

#pragma once

#include <vsg/all.h>

#include "subgraph.h"

namespace tired {

struct BoundingSubgraph;

struct Bounding final : public QObject {
    Q_OBJECT

public:
    Bounding( vsg::Viewer* viewer, QObject* parent = nullptr );

private:
    vsg::ref_ptr<BoundingSubgraph> _bounding{};
};

// ===============================================================================

struct BoundingSubgraph final : public Subgraph {
    BoundingSubgraph( vsg::Viewer* viewer );

    auto initPipeline() -> void override;
    auto initDrawCommand() -> void;

    friend Bounding;

private:
};

}  // namespace tired