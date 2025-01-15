
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
        instance_ = std::make_unique<vk::Instance>();
        surface_ = std::make_unique<vk::Surface>( display_, window_,
                                                  instance_->handle() );
        device_ = std::make_unique<vk::Device>( instance_->handle(),
                                                surface_->handle() );

        device_->pickAndCreateDevice( instance_.get(), 0 );

        // valid only after logical device creation
        const auto basePath = Config::instance()->getBasePath().string();
        shaderStorage_ =
            std::make_unique<vk::ShaderStorage>( device_->handle() );
        shaderStorage_->add( basePath + "/assets/shaders/001_shader_vert.spv",
                             "001_shader_vert" );
        shaderStorage_->add( basePath + "/assets/shaders/001_shader_frag.spv",
                             "001_shader_frag" );

        swapchain_ = std::make_unique<vk::Swapchain>( device_.get() );
        swapchain_->createSwapchain( device_.get(), surface_.get() );
        swapchain_->createImageViews( device_.get() );

        pipelineSimple_ =
            std::make_unique<vk::PiplineSimple>( device_->handle() );
        pipelineSimple_->initFixed();
        pipelineSimple_->initProgable(
            shaderStorage_->get( "001_shader_vert" ),
            shaderStorage_->get( "001_shader_frag" ) );
        pipelineSimple_->initLayout();
        pipelineSimple_->initRenderPass( swapchain_->imageFormat() );
        pipelineSimple_->initPipeline();
        pipelineSimple_->initFixed();

        swapchain_->createFramebuffers( device_.get(), pipelineSimple_.get() );

        commandPool_ = std::make_unique<vk::CommandPool>( device_->handle() );
        commandPool_->init( device_->graphicsFamily() );

        cBufs_.reserve( 2 );
        for ( auto i = 0; i < cBufs_.capacity(); ++i ) {
            cBufs_.push_back( std::make_unique<vk::CommandBuffer>(
                device_->handle(), commandPool_.get() ) );
        }

        createSyncObjects();

    } catch ( const std::runtime_error &e ) {
        throw std::runtime_error( e.what() );
    }
};

RenderVK::~RenderVK() {
    //commandPool_.reset( nullptr );
    for ( auto &buf : cBufs_ ) {
        buf.release();
    }

    for ( auto i = 0; i < 2; i++ ) {
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
    imageAvailableSemaphores_.resize( 2 );
    renderFinishedSemaphores_.resize( 2 );
    inFlightFences_.resize( 2 );

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for ( size_t i = 0; i < 2; i++ ) {
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
