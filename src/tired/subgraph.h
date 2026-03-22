
#pragma once

#include <vector>
#include <memory.h>

#include <QObject>

#include <vsg/all.h>

#include "scene_object/sceneobjectbase.h"

namespace tired {

struct Subgraph : QObject {
    Q_OBJECT

public:
    Subgraph( vsg::Viewer* viewer, QObject* parent = nullptr );

    virtual auto initPipeline() -> void = 0;

    auto stateGroup() -> vsg::ref_ptr<vsg::StateGroup>;

protected:
    auto link( std::shared_ptr<SceneObjectBase> object ) -> void;
    auto recompile() -> void;

signals:
    void nodeAdded();

protected:
    vsg::Viewer* _viewer{};

    vsg::ref_ptr<vsg::StateGroup> stateGroup_{};
    vsg::ref_ptr<vsg::Group> baseNode_{};
    std::vector<std::shared_ptr<SceneObjectBase>> _objectsList{};
};

}  // namespace tired
