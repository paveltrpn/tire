#include <vsg/all.h>
#include <vsg/app/Viewer.h>

#ifdef vsgXchange_FOUND
#include <vsgXchange/all.h>
#endif

#include "tired.h"

namespace tired {

Tired::Tired( QObject* parent )
    : QObject{ parent }
    , viewer_{ Viewer::create() } {};

auto Tired::viewer() -> vsg::ref_ptr<Viewer> {
    return viewer_;
}

auto Tired::manipulator() -> Manipulator* {
    return _manipulator;
}

auto Tired::inputHandler() -> InputHandler* {
    return _inputHandler;
}

auto Tired::camera() -> vsg::ref_ptr<vsg::Camera> {
    return camera_;
}

auto Tired::scenegraph() -> Scenegraph* {
    return _scenegraph;
}

auto Tired::init( Window* window, uint32_t width, uint32_t height ) -> void {
    // Setup the camera.
    {
        auto lookAt = vsg::LookAt::create( vsg::dvec3( 3.0, -8.0, 5.0 ), vsg::dvec3{ 0.0, 0.0, 0.0 },
                                           vsg::dvec3( 0.0, 0.0, 1.0 ) );

        vsg::ref_ptr<vsg::ProjectionMatrix> perspective =
            vsg::Perspective::create( 30.0, static_cast<double>( width ) / static_cast<double>( height ), 0.01, 500.0 );

        camera_ = vsg::Camera::create( perspective, lookAt, vsg::ViewportState::create( VkExtent2D{ width, height } ) );
    }

    // Setup scenegraph.
    {
        //
        _scenegraph = new Scenegraph{ viewer_, this };
    }

    // Setup manipulator and event handler objects.
    {
        _manipulator = new Manipulator{ camera_, nullptr, this };
        _manipulator->trackball()->addWindow( *window );
        _inputHandler = new InputHandler{ camera_, _scenegraph->root(), viewer_, this };
    }

    // Setup viewer object.
    {
        viewer_->addEventHandler( _manipulator->trackball() );
        viewer_->addEventHandler( _inputHandler->handler() );
        auto commandGraph = vsg::createCommandGraphForView( *window, camera_, _scenegraph->root() );
        viewer_->addRecordAndSubmitTaskAndPresentation( { commandGraph } );

        constexpr auto UPDATE_INTERVAL{ 8 };
        viewer_->setInterval( UPDATE_INTERVAL );

        constexpr auto CONTINOUS_UPDATE{ true };
        viewer_->continuousUpdate = CONTINOUS_UPDATE;
    }

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

    // Viewer compile.
    {
        //
        viewer_->compile();
    }

    // Add default cube.
    // _scenegraph->addExBox( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0 );
}

auto Tired::registerTypes() -> void {
    qRegisterMetaType<tired::SceneObjectTypeEnum>( "SceneObjectTypeEnum" );

    qRegisterMetaType<tired::SceneObjectData>( "SceneObjectData" );
    qRegisterMetaType<tired::BoxObjectData>( "BoxObject" );
    qRegisterMetaType<tired::SphereObjectData>( "SphereObjectData" );
    qRegisterMetaType<tired::MeshData>( "MeshData" );
}

}  // namespace tired
