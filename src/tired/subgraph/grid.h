
#pragma once

#include <vsg/all.h>

#include "subgraph.h"

namespace tired {

struct Grid final : Subgraph {
    Grid( vsg::Viewer* viewer );

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
