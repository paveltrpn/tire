
#pragma once

#include <QObject>

#include <vsg/all.h>

namespace tired {

struct Subgraph
    : QObject
    , vsg::StateGroup {
    Q_OBJECT

public:
    Subgraph()
        : QObject{}
        , vsg::StateGroup{} {}

    virtual auto initPipeline() -> void = 0;

private:
};

}  // namespace tired
