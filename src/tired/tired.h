
#pragma once

#include <memory>

#include <QObject>

#include <vsg/all.h>

#include "vk/context.h"
#include "qt_window/window.h"
#include "manipulator.h"
#include "inputhandler.h"
#include "scenegraph.h"

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
    auto scenegraph() -> Scenegraph*;

    auto registerTypes() -> void;

private:
    std::shared_ptr<vk::Context> context_{};

    vsg::ref_ptr<Viewer> viewer_{};
    vsg::ref_ptr<vsg::Camera> camera_{};

    Manipulator* _manipulator{};
    InputHandler* _inputHandler{};
    Scenegraph* _scenegraph{};
};

}  // namespace tired
