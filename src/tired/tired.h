
#pragma once

#include <memory>

#include <vsg/all.h>
#include <QObject>

#include "vk/context.h"
#include "qt_window/window.h"
#include "manipulator.h"
#include "inputhandler.h"
#include "basemesh/BasemeshSubgraph.h"
#include "obstacles/ObstaclesSubgraph.h"
#include "service_objects/ServiceObjectsSubgraph.h"

namespace tired {

struct Tired final : QObject {
    Q_OBJECT
public:
    Tired( QObject* parent = nullptr );

    auto init( Window* window, uint32_t width, uint32_t height ) -> void;

    auto viewer() -> vsg::ref_ptr<Viewer>;
    auto manipulator() -> Manipulator*;
    auto inputHandler() -> InputHandler*;
    auto rootNode() -> vsg::ref_ptr<vsg::Node>;
    auto camera() -> vsg::ref_ptr<vsg::Camera>;

    auto basemeshSubgraph() -> BasemeshSubgraph*;
    auto obstaclesSubgraph() -> ObstaclesSubgraph*;
    auto serviceObjectsSubgraph() -> ServiceObjectsSubgraph*;

    auto registerTypes() -> void;

private:
    std::shared_ptr<vk::Context> context_{};

    vsg::ref_ptr<Viewer> viewer_{};
    vsg::ref_ptr<vsg::Camera> camera_{};

    Manipulator* _manipulator{};
    InputHandler* _inputHandler{};

    vsg::ref_ptr<vsg::Group> theRoot_{};

    BasemeshSubgraph* basemeshSubgraph_{};
    ObstaclesSubgraph* obstaclesSubgraph_{};
    ServiceObjectsSubgraph* serviceObjectsSubgraph_{};
};

}  // namespace tired
