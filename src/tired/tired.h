
#pragma once

#include <vsg/all.h>

#include <QObject>

#include "qt_window/window.h"
#include "manipulator.h"
#include "basemesh/BasemeshSubgraph.h"
#include "obstacles/ObstaclesSubgraph.h"
#include "service_objects/ServiceObjectsSubgraph.h"

namespace tired {

struct Tired final : QObject {
    Q_OBJECT
public:
    Tired( QObject* parent = nullptr );

    auto initCamera( Window* window, uint32_t width, uint32_t height ) -> void;
    auto viewerCompile( int interval, bool continuousUpdate ) -> void;

    auto viewer() -> vsg::ref_ptr<Viewer>;
    auto manipulator() -> vsg::ref_ptr<Manipulator>;
    auto rootNode() -> vsg::ref_ptr<vsg::Node>;
    auto camera() -> vsg::ref_ptr<vsg::Camera>;

    auto basemeshSubgraph() -> BasemeshSubgraph*;
    auto obstaclesSubgraph() -> ObstaclesSubgraph*;
    auto serviceObjectsSubgraph() -> ServiceObjectsSubgraph*;

private:
    vsg::ref_ptr<Viewer> viewer_{};
    vsg::ref_ptr<vsg::Camera> camera_{};
    vsg::ref_ptr<Manipulator> manipulator_{};

    vsg::ref_ptr<vsg::Group> theRoot_{};

    BasemeshSubgraph* basemeshSubgraph_{};
    ObstaclesSubgraph* obstaclesSubgraph_{};
    ServiceObjectsSubgraph* serviceObjectsSubgraph_{};
};

}  // namespace tired
