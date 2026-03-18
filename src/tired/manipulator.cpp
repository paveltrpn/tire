
#include <QPoint>

#include "manipulator.h"

namespace tired {

Manipulator::Manipulator( vsg::ref_ptr<vsg::Camera> camera, vsg::ref_ptr<vsg::EllipsoidModel> ellipsoidModel )
    : QObject{}
    , _trackball{ new Trackball{ camera, ellipsoidModel } } {
}

auto Manipulator::trackball() -> const vsg::ref_ptr<Trackball> {
    return _trackball;
}

void Manipulator::setMousePos( QPoint value ) {
    mousePos_ = value;
    emit mousePosUpdated();
}

QPoint Manipulator::mousePos() {
    return mousePos_;
}

}  // namespace tired
