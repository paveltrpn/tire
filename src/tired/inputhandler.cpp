
#include "inputhandler.h"

namespace tired {

InputHandler::InputHandler( vsg::ref_ptr<vsg::Camera> camera )
    : vsg::Visitor{}
    , _camera{ camera } {
}

void InputHandler::apply( vsg::KeyPressEvent& keyPress ) {
}

void InputHandler::apply( vsg::KeyReleaseEvent& keyRelease ) {
}

void InputHandler::apply( vsg::FocusInEvent& focusIn ) {
}

void InputHandler::apply( vsg::FocusOutEvent& focusOut ) {
}

void InputHandler::apply( vsg::ButtonPressEvent& buttonPress ) {
}

void InputHandler::apply( vsg::ButtonReleaseEvent& buttonRelease ) {
}

void InputHandler::apply( vsg::MoveEvent& moveEvent ) {
}

void InputHandler::apply( vsg::ScrollWheelEvent& scrollWheel ) {
}

void InputHandler::apply( vsg::TouchDownEvent& touchDown ) {
}

void InputHandler::apply( vsg::TouchUpEvent& touchUp ) {
}

void InputHandler::apply( vsg::TouchMoveEvent& touchMove ) {
}

void InputHandler::apply( vsg::FrameEvent& frame ) {
}

}
