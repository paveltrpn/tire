
#pragma once

#include <QObject>

#include <vsg/all.h>

#include "subgraph/grid.h"
#include "subgraph/testbox.h"
#include "subgraph/sceneobject.h"
#include "subgraph/marker.h"
#include "subgraph/bounding.h"
#include "subgraph/gizmo.h"

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

    Grid* _grid{};
    Testbox* _testbox{};

    Bounding* _bounding{};
    Gizmo* _gizmo{};

    vsg::ref_ptr<SceneObjectSubgraph> _sceneObjectSubgraph{};
    vsg::ref_ptr<MarkerSubgraph> _markerSubgraph{};

    std::vector<std::shared_ptr<SceneObjectBase>> _objectsList{};
};

}  // namespace tired