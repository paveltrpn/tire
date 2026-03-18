
#include <print>

#include <QPoint>

#include "manipulator.h"

namespace tired {

Manipulator::Manipulator( vsg::ref_ptr<vsg::Camera> camera, vsg::ref_ptr<vsg::EllipsoidModel> ellipsoidModel )
    : QObject{}
    , _trackball{ new Trackball{ camera, ellipsoidModel } }
    , _inputHandler{ new InputHandler{ camera } } {
}

auto Manipulator::trackball() -> const vsg::ref_ptr<Trackball> {
    return _trackball;
}

auto Manipulator::inputHandler() -> const vsg::ref_ptr<InputHandler> {
    return _inputHandler;
}

void Manipulator::setMousePos( QPoint value ) {
    mousePos_ = value;
    emit mousePosUpdated();
}

QPoint Manipulator::mousePos() {
    return mousePos_;
}

}  // namespace tired
