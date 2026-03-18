
#include <QPoint>

#include "manipulator.h"

namespace tired {

Manipulator::Manipulator( vsg::ref_ptr<vsg::Camera> camera, vsg::ref_ptr<vsg::EllipsoidModel> ellipsoidModel )
    : QObject{}
    , _manipulator{ new vsg::Trackball{ camera, ellipsoidModel } } {
}

auto Manipulator::manipulator() -> const vsg::ref_ptr<vsg::Trackball> {
    return _manipulator;
}

// void Manipulator::apply( vsg::MoveEvent& moveEvent ) {
//     setMousePos( { moveEvent.x, moveEvent.y } );

//     _manipulator->apply( moveEvent );
// }

void Manipulator::setMousePos( QPoint value ) {
    mousePos_ = value;
    emit mousePosUpdated();
}

QPoint Manipulator::mousePos() {
    return mousePos_;
}

}  // namespace tired
