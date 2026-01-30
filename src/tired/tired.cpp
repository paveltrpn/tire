#include <vsg/all.h>
#include <vsg/app/Viewer.h>

#include <iostream>
#include <print>

#ifdef vsgXchange_FOUND
#include <vsgXchange/all.h>
#endif

#include <vsgQt/Window.h>

#include "tired.h"
#include "box/box.h"
#include "box/ex_box.h"

namespace tired {

Tired::Tired( QObject* parent )
    : QObject{ parent }
    , viewer_{ vsgQt::Viewer::create() }
    , theRoot_{ new vsg::Group{} } {

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

auto Tired::camera() -> vsg::ref_ptr<vsg::Camera> {
    return camera_;
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

    auto box = vsg::ref_ptr<Box>( new Box{ vsg::dvec3{ 1000.0, 0.0, 0.0 }, vsg::dvec3{ 500.0, 500.0, 500.0 } } );
    theRoot_->addChild( box );

    auto exbox = vsg::ref_ptr<ExBox>( new ExBox{ vsg::dvec3{}, vsg::dvec3{ 500.0, 500.0, 500.0 } } );
    theRoot_->addChild( exbox );

    return 0;
}

Q_INVOKABLE void Tired::setCameraToBounds() {
    qDebug() << "first call";
}

auto Tired::initWindow( vsg::ref_ptr<vsg::WindowTraits> traits, QWindow* parent ) -> vsgQt::Window* {
    auto window = new vsgQt::Window( viewer_, traits, parent );

    window->setTitle( "title" );
    window->initializeWindow();

    // if this is the first window to be created, use its device for future window creation.
    if ( !traits->device ) {
        traits->device = window->windowAdapter->getOrCreateDevice();
    }

    // compute the bounds of the scene graph to help position camera
    vsg::ComputeBounds computeBounds;
    theRoot_->accept( computeBounds );
    // vsg::dvec3 centre = ( computeBounds.bounds.min + computeBounds.bounds.max ) * 0.8;
    auto centre = vsg::dvec3{ 0.0, 0.0, 0.0 };
    // double radius = vsg::length( computeBounds.bounds.max - computeBounds.bounds.min ) * 0.6;
    double radius = 5000.0 * 3.0;

    std::println( "{} {} {}", centre.x, centre.y, centre.z );

    double nearFarRatio = 0.001;

    uint32_t width = window->traits->width;
    uint32_t height = window->traits->height;

    {
        // set up the camera
        auto lookAt = vsg::LookAt::create( vsg::dvec3( 0.0, -radius, 0.0 ), centre, vsg::dvec3( 0.0, 0.0, 1.0 ) );

        vsg::ref_ptr<vsg::ProjectionMatrix> perspective = vsg::Perspective::create(
            30.0, static_cast<double>( width ) / static_cast<double>( height ), nearFarRatio * radius, radius * 4.5 );

        camera_ = vsg::Camera::create( perspective, lookAt, vsg::ViewportState::create( VkExtent2D{ width, height } ) );
    }

    trackball_ = vsg::Trackball::create( camera_, nullptr );
    trackball_->addWindow( *window );

    viewer_->addEventHandler( trackball_ );

    auto commandGraph = vsg::createCommandGraphForView( *window, camera_, theRoot_ );

    viewer_->addRecordAndSubmitTaskAndPresentation( { commandGraph } );

    return window;
}

}  // namespace tired
