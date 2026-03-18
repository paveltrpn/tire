
#include "trackball.h"

namespace tired {

Trackball::Trackball( vsg::ref_ptr<vsg::Camera> camera, vsg::ref_ptr<vsg::EllipsoidModel> ellipsoidModel )
    : vsg::Trackball{ camera, ellipsoidModel } {
}

void Trackball::apply( vsg::MoveEvent& moveEvent ) {
    // setMousePos( { moveEvent.x, moveEvent.y } );
    // _manipulator->apply( moveEvent );

    vsg::Trackball::apply( moveEvent );
}

}  // namespace tired
