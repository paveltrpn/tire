
#pragma once

#include <QObject>
#include <QPoint>

#include <vsg/all.h>
#include <vsg/app/Viewer.h>
#include <vsg/ui/Keyboard.h>
#include <vsg/ui/PointerEvent.h>
#include <vsg/ui/ScrollWheelEvent.h>
#include <vsg/ui/TouchEvent.h>

#include "trackball.h"
#include "inputhandler.h"

namespace tired {

// ==================================================================

struct Manipulator final : QObject {
    Q_OBJECT

    Q_PROPERTY( QPoint mousePos READ mousePos WRITE setMousePos NOTIFY mousePosUpdated )

public:
    Manipulator( vsg::ref_ptr<vsg::Camera> camera, vsg::ref_ptr<vsg::EllipsoidModel> ellipsoidModel = {} );

    auto trackball() -> const vsg::ref_ptr<Trackball>;
    auto inputHandler() -> const vsg::ref_ptr<InputHandler>;

public:
    void setMousePos( QPoint value );
    QPoint mousePos();

signals:
    void mousePosUpdated();

private:
    vsg::ref_ptr<Trackball> _trackball{};
    vsg::ref_ptr<InputHandler> _inputHandler{};

    QPoint mousePos_{};
};

}  // namespace tired
