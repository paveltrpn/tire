
#pragma once

#include <vsg/all.h>

#include "subgraph.h"

namespace tired {

struct GridSubgraph;

struct Grid final : public QObject {
    Q_OBJECT

public:
    Grid( vsg::Viewer* viewer, QObject* parent = nullptr );

    auto grid() const -> vsg::ref_ptr<GridSubgraph>;

private:
    vsg::ref_ptr<GridSubgraph> _grid{};
};

// ===============================================================================

struct GridSubgraph final : Subgraph {
    GridSubgraph( vsg::Viewer* viewer );

    auto initPipeline() -> void override;
    auto initDrawCommand() -> void;

private:
    float _gridSize{ 0.5f };
    float _lineThickness{ 0.015f };
    float _maxRange{ 100.0f };
    float _zoomSensitivity{ 0.05f };
    vsg::vec3 _colorMajor{ 0.7f, 0.2f, 0.2f };
    vsg::vec3 _colorMinor{ 0.2f, 0.2f, 0.5f };
    float _majorDivisor{ 5.0f };

    vsg::ref_ptr<vsg::floatArray> _gridBufUniformValue{};
};

}  // namespace tired
