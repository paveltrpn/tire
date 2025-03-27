
#include <print>
#include <format>
#include <set>

#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include "rendervk.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_RENDERVK_CPP{ true };
#include "algebra/matrix4.h"

import config;

namespace tire {

RenderVK::RenderVK()
    : Render{} {
    try {
        const auto configHandle = Config::instance();
        const auto basePath = configHandle->getBasePath().string();
        context_ = std::make_unique<vk::Context>( display_, window_ );
        context_->init();

        piplineVertexBuffer_ =
            std::make_unique<vk::PiplineVertexBuffer>( context_.get() );
        piplineVertexBuffer_->initPipeline(
            { basePath + "/assets/shaders/spirv/vk_vertexBuffer_VERTEX.spv",
              basePath +
                  "/assets/shaders/spirv/vk_vertexBuffer_FRAGMENT.spv" } );

        context_->createFramebuffers( piplineVertexBuffer_.get() );

    } catch ( const std::runtime_error &e ) {
        throw std::runtime_error( e.what() );
    }
};

void RenderVK::scene( const std::filesystem::path &path ) {
    scene_ = std::make_shared<vk::Scene>( path, context_.get(),
                                          piplineVertexBuffer_.get() );
}

void RenderVK::setSwapInterval( int interval ) {

};

}  // namespace tire
