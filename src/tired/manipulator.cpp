
#include <QPoint>

#include "manipulator.h"

namespace tired {

Manipulator::Manipulator( vsg::ref_ptr<vsg::Camera> camera, vsg::ref_ptr<vsg::EllipsoidModel> ellipsoidModel )
    : QObject{}
    , vsg::Trackball{ camera, ellipsoidModel } {
}

void Manipulator::apply( vsg::MoveEvent& moveEvent ) {
    setMousePos( { moveEvent.x, moveEvent.y } );

    vsg::Trackball::apply( moveEvent );
}

void Manipulator::setMousePos( QPoint value ) {
    mousePos_ = value;
    emit mousePosUpdated();
}

QPoint Manipulator::mousePos() {
    return mousePos_;
}

}  // namespace tired
