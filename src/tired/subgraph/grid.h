
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

    Q_INVOKABLE void setGridSize( float value );
    Q_INVOKABLE void setLineThickness( float value );
    Q_INVOKABLE void setMaxRange( float value );
    Q_INVOKABLE void setZoomSensitivity( float value );
    Q_INVOKABLE void setColorMajor( float r, float g, float b );
    Q_INVOKABLE void setColorMinor( float r, float g, float b );
    Q_INVOKABLE void setMajorDivisor( float value );

    Q_INVOKABLE void setGridScale( float value );
    Q_INVOKABLE void setGridZOffset( float value );

private:
    vsg::ref_ptr<GridSubgraph> _grid{};
};

// ===============================================================================

struct GridSubgraph final : Subgraph {
    GridSubgraph( vsg::Viewer* viewer );

    auto initPipeline() -> void override;
    auto initDrawCommand() -> void;

    auto recompile() -> void override;

    friend Grid;

private:
    auto updateGridBufUniformValue() -> void;
    auto updatePlaneBufUniformValue() -> void;

private:
    float _gridSize{ 0.8f };
    float _lineThickness{ 0.015f };
    float _maxRange{ 100.0f };
    float _zoomSensitivity{ 0.05f };
    vsg::vec3 _colorMajor{ 0.7f, 0.2f, 0.2f };
    vsg::vec3 _colorMinor{ 0.2f, 0.2f, 0.5f };
    float _majorDivisor{ 5.0f };

    float _gridScale{ 10.0 };
    float _gridZOffset{ 0.0 };

    vsg::ref_ptr<vsg::floatArray> _gridBufUniformValue{};
    vsg::ref_ptr<vsg::floatArray> _planeBufUniformValue{};
};

}  // namespace tired
