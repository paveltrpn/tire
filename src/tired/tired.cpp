#include <vsg/all.h>
#include <vsg/app/Viewer.h>

#include <iostream>

#ifdef vsgXchange_FOUND
#include <vsgXchange/all.h>
#endif

#include <vsgQt/Window.h>

#include "tired.h"

namespace tired {

Tired::Tired( QObject* parent )
    : QObject{ parent }
    , viewer_{ vsgQt::Viewer::create() }
    , theRoot_{ new vsg::Group{} } {
    //

    auto mainMatrixTranform =
        vsg::ref_ptr<vsg::MatrixTransform>{ new vsg::MatrixTransform{ vsg::scale( 10.0, 10.0, 10.0 ) } };
};

auto Tired::viewerCompile( int interval, bool continuousUpdate ) -> void {
    if ( interval >= 0 ) {
        viewer_->setInterval( interval );
    }

    viewer_->continuousUpdate = continuousUpdate;

    viewer_->addEventHandler( vsg::CloseHandler::create( viewer_ ) );
    viewer_->compile();
}

auto Tired::viewer() -> vsg::ref_ptr<vsgQt::Viewer> {
    return viewer_;
}

auto Tired::rootNode() -> vsg::ref_ptr<vsg::Node> {
    return theRoot_;
}

auto Tired::loadTestScene( vsg::Path filename, vsg::ref_ptr<vsg::Options> options ) -> int {
    auto scene = vsg::read_cast<vsg::Node>( filename, options );
    if ( !scene ) {
        std::cout << "Failed to load a valid scene graph. Please specify a valid 3d "
                     "model or image file on the command line."
                  << std::endl;
        return 1;
    }

    auto mainMatrixTranform =
        vsg::ref_ptr<vsg::MatrixTransform>{ new vsg::MatrixTransform{ vsg::scale( 10.0, 10.0, 10.0 ) } };

    mainMatrixTranform->addChild( scene );
    theRoot_->addChild( mainMatrixTranform );

    return 0;
}

}  // namespace tired
