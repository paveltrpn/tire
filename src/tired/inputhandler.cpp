
#include "inputhandler.h"

namespace tired {

Handler::Handler( vsg::ref_ptr<vsg::Camera> camera, InputHandler* inputHandler )
    : vsg::Visitor{}
    , _camera{ camera }
    , _inputHandler{ inputHandler } {
}

void Handler::apply( vsg::KeyPressEvent& keyPress ) {
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

// =======================================================================

InputHandler::InputHandler( vsg::ref_ptr<vsg::Camera> camera )
    : _handler{ new Handler{ camera, this } } {
}

auto InputHandler::handler() -> const vsg::ref_ptr<Handler> {
    return _handler;
}

void InputHandler::setMousePos( QPoint value ) {
    mousePos_ = value;
    emit mousePosUpdated();
}

QPoint InputHandler::mousePos() {
    return mousePos_;
}

}  // namespace tired
