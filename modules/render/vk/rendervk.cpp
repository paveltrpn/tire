
#include <print>
#include <format>

#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include <set>

#include "rendervk.h"

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_RENDERVK_CPP{ true };

#include "algebra/matrix4.h"

namespace tire {

RenderVK::RenderVK()
    : Render{} {
    try {
        // Create instance (application wide)
        instance_ = std::make_unique<vk::Instance>();

        // Create surface (application wide)
        surface_ =
            std::make_unique<vk::Surface>( display_, window_, instance_.get() );

        // Create logical device and obtain physical devices (application wide)
        device_ =
            std::make_unique<vk::Device>( instance_.get(), surface_.get() );
        device_->pickAndCreateDevice();

        const auto configHandle = Config::instance();
        const auto basePath = configHandle->getBasePath().string();

        // Create swapchain (application wide)
        swapchain_ =
            std::make_unique<vk::Swapchain>( device_.get(), surface_.get() );
        swapchain_->createSwapchain();
        swapchain_->createImageViews();

        // Create pipeline (maybe add more)
        pipelineSimple_ = std::make_unique<vk::PiplineSimple>( device_.get() );
        pipelineSimple_->initPipeline(
            { basePath + "/assets/shaders/vk_simple_tri_VERTEX.spv",
              basePath + "/assets/shaders/vk_simple_tri_FRAGMENT.spv" } );

        piplineMatrixReady =
            std::make_unique<vk::PiplineMatrixReady>( device_.get() );
        piplineMatrixReady->initPipeline(
            { basePath + "/assets/shaders/vk_simple_box_VERTEX.spv",
              basePath + "/assets/shaders/vk_simple_box_FRAGMENT.spv" } );

        swapchain_->createFramebuffers( piplineMatrixReady.get() );

        // Create command pool (application wide)
        commandPool_ = std::make_unique<vk::CommandPool>( device_.get() );

        // Create command buffers for each frame
        cBufs_.reserve( 3 );
        for ( auto i = 0; i < cBufs_.capacity(); ++i ) {
            cBufs_.push_back( std::make_unique<vk::RenderFromShader>(
                device_.get(), commandPool_.get() ) );
        }

        dummyCmd_ = std::make_unique<vk::DummyCommand>( device_.get(),
                                                        commandPool_.get() );

        present_ =
            std::make_unique<vk::Present>( device_.get(), swapchain_.get() );

        presentSync_ =
            std::make_unique<vk::PresentSynchronization<3>>( device_.get() );

    } catch ( const std::runtime_error &e ) {
        throw std::runtime_error( e.what() );
    }
};

void RenderVK::scene( const std::filesystem::path &path ) {
    scene_ = std::make_shared<vk::Scene>( path, loop_ );
}

void RenderVK::setSwapInterval( int interval ){

};

}  // namespace tire
