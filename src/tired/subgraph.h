
#pragma once

#include <QObject>

#include <vsg/all.h>

namespace tired {

struct Subgraph : QObject {
    Q_OBJECT

public:
    Subgraph();

    virtual auto initPipeline() -> void = 0;

    auto stateGroup() -> vsg::ref_ptr<vsg::StateGroup>;

signals:
    void nodeAdded();

protected:
    vsg::ref_ptr<vsg::StateGroup> stateGroup_{};
};

}  // namespace tired
