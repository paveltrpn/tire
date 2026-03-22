
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
    Subgraph( QObject *parent = nullptr );

    virtual auto initPipeline() -> void = 0;

    auto stateGroup() -> vsg::ref_ptr<vsg::StateGroup>;

signals:
    void nodeAdded();

protected:
    vsg::ref_ptr<vsg::StateGroup> stateGroup_{};
    std::vector<std::shared_ptr<SceneObjectBase>> _objectsList{};
};

}  // namespace tired
