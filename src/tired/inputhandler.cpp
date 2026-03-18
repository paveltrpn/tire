
#include <print>

#include "inputhandler.h"

namespace tired {

Handler::Handler( vsg::ref_ptr<vsg::Camera> camera, InputHandler* inputHandler )
    : vsg::Visitor{}
    , _camera{ camera }
    , _inputHandler{ inputHandler } {
}

void Handler::apply( vsg::KeyPressEvent& keyPress ) {
    if ( closeKey != vsg::KEY_Undefined && keyPress.keyBase == closeKey ) {
        close();
    }
}

void Handler::apply( vsg::KeyReleaseEvent& keyRelease ) {
}

void Handler::apply( vsg::FocusInEvent& focusIn ) {
}

void Handler::apply( vsg::FocusOutEvent& focusOut ) {
}

void Handler::apply( vsg::ButtonPressEvent& buttonPress ) {
}

void Handler::apply( vsg::ButtonReleaseEvent& buttonRelease ) {
}

void Handler::apply( vsg::MoveEvent& moveEvent ) {
    _inputHandler->setMousePos( { moveEvent.x, moveEvent.y } );
}

void Handler::apply( vsg::ScrollWheelEvent& scrollWheel ) {
}

void Handler::apply( vsg::TouchDownEvent& touchDown ) {
}

void Handler::apply( vsg::TouchUpEvent& touchUp ) {
}

void Handler::apply( vsg::TouchMoveEvent& touchMove ) {
}

void Handler::apply( vsg::FrameEvent& frame ) {
}

void Handler::apply( vsg::CloseWindowEvent& ) {
    close();
}

void Handler::apply( vsg::TerminateEvent& ) {
    close();
}

void Handler::close() {
    // take a ref_ptr<> of the observer_ptr<> to be able to safely access it
    vsg::ref_ptr<vsg::Viewer> viewer = _inputHandler->viewer();
    if ( viewer ) {
        viewer->close();
    }
}

// =======================================================================

InputHandler::InputHandler( vsg::ref_ptr<vsg::Camera> camera, vsg::ref_ptr<vsg::Viewer> viwer )
    : _handler{ new Handler{ camera, this } }
    , _viewer{ viwer } {
}

auto InputHandler::handler() -> const vsg::ref_ptr<Handler> {
    return _handler;
}

auto InputHandler::viewer() -> const vsg::ref_ptr<vsg::Viewer> {
    return _viewer;
};

void InputHandler::setMousePos( QPoint value ) {
    mousePos_ = value;
    emit mousePosUpdated();
}

QPoint InputHandler::mousePos() {
    return mousePos_;
}

}  // namespace tired
