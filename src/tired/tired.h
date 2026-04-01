
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

    Q_PROPERTY( QObject* manipulator READ manipulator NOTIFY manipulatorChanged FINAL )
    Q_PROPERTY( QObject* inputHandler READ inputHandler NOTIFY inputHandlerChanged FINAL )
    Q_PROPERTY( QObject* scenegraph READ scenegraph NOTIFY scenegraphChanged FINAL )

    Q_PROPERTY( QObject* grid READ grid NOTIFY gridChanged FINAL )

public:
    Tired( QObject* parent = nullptr );

    auto init( Window* window, uint32_t width, uint32_t height ) -> void;

    auto viewer() -> vsg::ref_ptr<Viewer>;
    auto rootNode() -> vsg::ref_ptr<vsg::Node>;
    auto camera() -> vsg::ref_ptr<vsg::Camera>;

    auto manipulator() const -> QObject*;
    auto inputHandler() const -> QObject*;
    auto scenegraph() const -> QObject*;
    auto grid() const -> QObject*;

    auto registerTypes() -> void;

signals:
    void manipulatorChanged();
    void inputHandlerChanged();
    void scenegraphChanged();
    void gridChanged();

private:
    std::shared_ptr<vk::Context> context_{};

    vsg::ref_ptr<Viewer> viewer_{};
    vsg::ref_ptr<vsg::Camera> camera_{};

    Manipulator* _manipulator{};
    InputHandler* _inputHandler{};
    Scenegraph* _scenegraph{};
};

}  // namespace tired
