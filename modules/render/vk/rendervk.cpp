
#include <print>
#include <format>

#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include <set>

#include "rendervk.h"

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_RENDERVK_CPP{ true };

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
        pipelineSimple_->initFixed();
        pipelineSimple_->initProgable(
            { basePath + "/assets/shaders/vk_simple_tri_VERTEX.spv",
              basePath + "/assets/shaders/vk_simple_tri_FRAGMENT.spv" } );
        pipelineSimple_->initPipeline();

        swapchain_->createFramebuffers( pipelineSimple_.get() );

        // Create command pool (application wide)
        commandPool_ = std::make_unique<vk::CommandPool>( device_.get() );

        // Create command buffers for each frame
        cBufs_.reserve( 3 );
        for ( auto i = 0; i < cBufs_.capacity(); ++i ) {
            cBufs_.push_back(
                std::make_unique<
                    vk::RenderCommandBufferBase<vk::RenderFromShader>>(
                    device_.get(), commandPool_.get() ) );
        }

        present_ =
            std::make_unique<vk::Present>( device_.get(), swapchain_.get() );

        // Create synchronize objects
        createSyncObjects();

    } catch ( const std::runtime_error &e ) {
        throw std::runtime_error( e.what() );
    }
};

RenderVK::~RenderVK() {
    for ( auto &buf : cBufs_ ) {
        buf.release();
    }

    for ( auto i = 0; i < 3; i++ ) {
        vkDestroySemaphore( device_->handle(), imageAvailableSemaphores_[i],
                            nullptr );
        vkDestroySemaphore( device_->handle(), renderFinishedSemaphores_[i],
                            nullptr );
        vkDestroyFence( device_->handle(), inFlightFences_[i], nullptr );
    }
};

void RenderVK::scene( const std::filesystem::path &path ) {
    scene_ = std::make_shared<vk::Scene>( path, loop_ );
}

void RenderVK::setSwapInterval( int interval ){

};

void RenderVK::createSyncObjects() {
    imageAvailableSemaphores_.resize( 3 );
    renderFinishedSemaphores_.resize( 3 );
    inFlightFences_.resize( 3 );

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for ( size_t i = 0; i < 3; i++ ) {
        if ( vkCreateSemaphore( device_->handle(), &semaphoreInfo, nullptr,
                                &imageAvailableSemaphores_[i] ) != VK_SUCCESS ||
             vkCreateSemaphore( device_->handle(), &semaphoreInfo, nullptr,
                                &renderFinishedSemaphores_[i] ) != VK_SUCCESS ||
             vkCreateFence( device_->handle(), &fenceInfo, nullptr,
                            &inFlightFences_[i] ) != VK_SUCCESS ) {
            throw std::runtime_error( "failed to create semaphores!" );
        }
    }
}

}  // namespace tire
