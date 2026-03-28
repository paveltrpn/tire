
#pragma once

#include <vector>
#include <memory.h>

#include <QObject>

#include <vsg/all.h>

#include "../scene_object/sceneobjectbase.h"

namespace tired {

struct Subgraph : vsg::Group {
    Subgraph( vsg::Viewer* viewer );

    virtual auto initPipeline() -> void = 0;

    auto recompile() -> void;

protected:
    vsg::Viewer* _viewer{};
    vsg::ref_ptr<vsg::StateGroup> stateGroup_{};
};

}  // namespace tired
