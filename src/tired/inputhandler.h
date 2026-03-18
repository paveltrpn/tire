
#pragma once

#include <QObject>
#include <QPoint>

#include <vsg/all.h>
#include <vsg/app/Viewer.h>
#include <vsg/ui/Keyboard.h>
#include <vsg/ui/PointerEvent.h>
#include <vsg/ui/ScrollWheelEvent.h>
#include <vsg/ui/TouchEvent.h>

namespace tired {

struct InputHandler final : vsg::Visitor {
    InputHandler( vsg::ref_ptr<vsg::Camera> camera );

    void apply( vsg::KeyPressEvent& keyPress ) override;
    void apply( vsg::KeyReleaseEvent& keyRelease ) override;
    void apply( vsg::FocusInEvent& focusIn ) override;
    void apply( vsg::FocusOutEvent& focusOut ) override;
    void apply( vsg::ButtonPressEvent& buttonPress ) override;
    void apply( vsg::ButtonReleaseEvent& buttonRelease ) override;
    void apply( vsg::MoveEvent& moveEvent ) override;
    void apply( vsg::ScrollWheelEvent& scrollWheel ) override;
    void apply( vsg::TouchDownEvent& touchDown ) override;
    void apply( vsg::TouchUpEvent& touchUp ) override;
    void apply( vsg::TouchMoveEvent& touchMove ) override;
    void apply( vsg::FrameEvent& frame ) override;

private:
    vsg::ref_ptr<vsg::Camera> _camera{};
};

}
