
#pragma once

#include <QObject>

#include <vsg/all.h>

#include "subgraph/grid.h"
#include "subgraph/testbox.h"
#include "subgraph/basemesh.h"
#include "subgraph/obstacles.h"

namespace tired {

struct Scenegraph final : public QObject {
    Q_OBJECT

public:
    Scenegraph( vsg::Viewer* viewer, QObject* parent = nullptr );

    auto root() -> vsg::ref_ptr<vsg::Group>;

    Q_INVOKABLE void addExBox( float px, float py, float pz, float rx, float ry, float rz, float sx, float sy,
                               float sz );

    auto grid() const -> Grid*;

private:
    vsg::ref_ptr<vsg::Group> _root{};
    vsg::Viewer* _viewer;

    vsg::ref_ptr<Testbox> _testbox{};
    Grid* _grid{};
    vsg::ref_ptr<BasemeshSubgraph> _basemeshSubgraph{};
    vsg::ref_ptr<ObstaclesSubgraph> _obstaclesSubgraph{};

    std::vector<std::shared_ptr<SceneObjectBase>> _objectsList{};
};

}  // namespace tired