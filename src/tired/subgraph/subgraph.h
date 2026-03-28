
#pragma once

#include <memory.h>

#include <QObject>

#include <vsg/all.h>

#include "../scene_object/sceneobjectbase.h"

namespace tired {

struct Subgraph : vsg::Group {
    Subgraph( vsg::Viewer* viewer );

    virtual auto initPipeline() -> void = 0;

    auto recompile() -> void;

    auto link( std::shared_ptr<SceneObjectBase> object ) -> void;

protected:
    vsg::Viewer* _viewer{};
    vsg::ref_ptr<vsg::StateGroup> _stateGroup{};
    vsg::ref_ptr<vsg::Group> _baseNode{};
};

}  // namespace tired
