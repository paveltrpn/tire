#include <vsg/all.h>
#include <vsg/app/Viewer.h>

#ifdef vsgXchange_FOUND
#include <vsgXchange/all.h>
#endif

#include "tired.h"
#include "box/ex_box.h"

namespace tired {

Tired::Tired( QObject* parent )
    : QObject{ parent }
    , viewer_{ Viewer::create() }
    , theRoot_{ new vsg::Group{} } {};

auto Tired::viewerCompile( int interval, bool continuousUpdate ) -> void {
    if ( interval >= 0 ) {
        viewer_->setInterval( interval );
    }

    viewer_->continuousUpdate = continuousUpdate;

    viewer_->addEventHandler( vsg::CloseHandler::create( viewer_ ) );
    viewer_->compile();
}

auto Tired::viewer() -> vsg::ref_ptr<Viewer> {
    return viewer_;
}

auto Tired::manipulator() -> vsg::ref_ptr<Manipulator> {
    return manipulator_;
}

auto Tired::rootNode() -> vsg::ref_ptr<vsg::Node> {
    return theRoot_;
}

auto Tired::camera() -> vsg::ref_ptr<vsg::Camera> {
    return camera_;
}

Q_INVOKABLE void Tired::addExBox( float px, float py, float pz, float rx, float ry, float rz, float sx, float sy,
                                  float sz ) {
    auto exbox = vsg::ref_ptr<ExBox>(
        new ExBox{ vsg::dvec3{ px, py, pz }, vsg::dvec3{ rx, ry, rz }, vsg::dvec3{ sx, sy, sz } } );
    basemeshSubgraph_->addChild( exbox );

    viewer_->compileManager->compile( basemeshSubgraph_ );
    vsg::CompileResult res{};
    vsg::updateViewer( *viewer_, res );
}

auto Tired::initCamera( Window* window, uint32_t width, uint32_t height ) -> void {
    // set up the camera
    auto lookAt =
        vsg::LookAt::create( vsg::dvec3( 0.0, -4.0, 1.5 ), vsg::dvec3{ 0.0, 0.0, 0.0 }, vsg::dvec3( 0.0, 0.0, 1.0 ) );

    vsg::ref_ptr<vsg::ProjectionMatrix> perspective =
        vsg::Perspective::create( 30.0, static_cast<double>( width ) / static_cast<double>( height ), 0.01, 500.0 );

    camera_ = vsg::Camera::create( perspective, lookAt, vsg::ViewportState::create( VkExtent2D{ width, height } ) );

    manipulator_ = vsg::ref_ptr<Manipulator>( new Manipulator{ camera_, nullptr } );
    manipulator_->addWindow( *window );

    viewer_->addEventHandler( manipulator_ );

    auto commandGraph = vsg::createCommandGraphForView( *window, camera_, theRoot_ );

    viewer_->addRecordAndSubmitTaskAndPresentation( { commandGraph } );

    basemeshSubgraph_ = vsg::ref_ptr<BasemeshSubgraph>( new BasemeshSubgraph{} );
    basemeshSubgraph_->initPipeline();

    theRoot_->addChild( basemeshSubgraph_ );

    auto box = vsg::ref_ptr<ExBox>(
        new ExBox{ vsg::dvec3{ 0.0, 0.0, 0.0 }, vsg::dvec3{ 0.0, 0.0, 0.0 }, vsg::dvec3{ 1.0, 1.0, 1.0 } } );
    basemeshSubgraph_->addChild( box );
}

}  // namespace tired
