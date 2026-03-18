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

auto Tired::viewer() -> vsg::ref_ptr<Viewer> {
    return viewer_;
}

auto Tired::manipulator() -> Manipulator* {
    return _manipulator;
}

auto Tired::rootNode() -> vsg::ref_ptr<vsg::Node> {
    return theRoot_;
}

auto Tired::camera() -> vsg::ref_ptr<vsg::Camera> {
    return camera_;
}

auto Tired::basemeshSubgraph() -> BasemeshSubgraph* {
    return basemeshSubgraph_;
}

auto Tired::obstaclesSubgraph() -> ObstaclesSubgraph* {
    return obstaclesSubgraph_;
}

auto Tired::serviceObjectsSubgraph() -> ServiceObjectsSubgraph* {
    return serviceObjectsSubgraph_;
}

auto Tired::init( Window* window, uint32_t width, uint32_t height ) -> void {
    // set up the camera
    auto lookAt =
        vsg::LookAt::create( vsg::dvec3( 0.0, -4.0, 1.5 ), vsg::dvec3{ 0.0, 0.0, 0.0 }, vsg::dvec3( 0.0, 0.0, 1.0 ) );

    vsg::ref_ptr<vsg::ProjectionMatrix> perspective =
        vsg::Perspective::create( 30.0, static_cast<double>( width ) / static_cast<double>( height ), 0.01, 500.0 );

    camera_ = vsg::Camera::create( perspective, lookAt, vsg::ViewportState::create( VkExtent2D{ width, height } ) );

    _manipulator = new Manipulator{ camera_, nullptr };
    _manipulator->trackball()->addWindow( *window );

    viewer_->addEventHandler( _manipulator->trackball() );

    auto commandGraph = vsg::createCommandGraphForView( *window, camera_, theRoot_ );

    viewer_->addRecordAndSubmitTaskAndPresentation( { commandGraph } );

    // Borrow vulkan data from VSG.
    {
        const auto wa = window->windowAdapter;
        const auto instance = wa->getInstance()->vk();
        const auto physicalDevice = wa->getPhysicalDevice()->vk();
        const auto device = wa->getDevice()->vk();
        const auto surface = wa->getSurface()->vk();
        const auto rp = wa->getRenderPass()->vk();

        context_ = std::make_shared<vk::Context>( instance, physicalDevice, device, surface, rp, 0, 0 );
    }

    basemeshSubgraph_ = new BasemeshSubgraph{ this };
    basemeshSubgraph_->initPipeline();

    obstaclesSubgraph_ = new ObstaclesSubgraph{ this };
    obstaclesSubgraph_->initPipeline();

    serviceObjectsSubgraph_ = new ServiceObjectsSubgraph{ this };
    serviceObjectsSubgraph_->initPipeline();

    connect( basemeshSubgraph_, &Subgraph::nodeAdded, this, [this]() {
        viewer_->compileManager->compile( basemeshSubgraph_->stateGroup() );
        vsg::CompileResult res{};
        vsg::updateViewer( *viewer_, res );
    } );

    theRoot_->addChild( basemeshSubgraph_->stateGroup() );
    theRoot_->addChild( obstaclesSubgraph_->stateGroup() );
    theRoot_->addChild( serviceObjectsSubgraph_->stateGroup() );

    auto box = vsg::ref_ptr<ExBox>(
        new ExBox{ vsg::dvec3{ 0.0, 0.0, 0.0 }, vsg::dvec3{ 0.0, 0.0, 0.0 }, vsg::dvec3{ 1.0, 1.0, 1.0 } } );
    basemeshSubgraph_->addChild( box );

    // Viewer compile.
    constexpr auto UPDATE_INTERVAL{ 8 };
    viewer_->setInterval( UPDATE_INTERVAL );

    constexpr auto CONTINOUS_UPDATE{ true };
    viewer_->continuousUpdate = CONTINOUS_UPDATE;

    viewer_->addEventHandler( vsg::CloseHandler::create( viewer_ ) );
    viewer_->compile();
}

}  // namespace tired
