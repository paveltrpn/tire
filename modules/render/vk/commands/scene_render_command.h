
#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <type_traits>

#include "../pipelines/pipeline.h"
#include "../command_pool.h"
#include "algebra/matrix4.h"
#include "image/color.h"

namespace tire::vk {
struct SceneRenderCommand final {
    SceneRenderCommand( const vk::Device *device, const CommandPool *pool );

    SceneRenderCommand( const SceneRenderCommand &other ) = delete;
    SceneRenderCommand( SceneRenderCommand &&other ) = delete;
    SceneRenderCommand &operator=( const SceneRenderCommand &other ) = delete;
    SceneRenderCommand &operator=( SceneRenderCommand &&other ) = delete;

    ~SceneRenderCommand() { clean(); }

    void clean();

    void reset();
    void begin( const vk::Pipeline *pipeline, VkFramebuffer framebuffer,
                algebra::matrix4f view );
    void draw( const std::vector<VkBuffer> &vertexBuffer,
               const std::vector<VkDeviceSize> &verteciesOffsets,
               uint32_t verteciesCount );
    void end( VkSemaphore waitSemaphores, VkSemaphore signalSemaphores,
              VkFence fence );

protected:
    // Propertis set on "command" creation and stay same trough lifetime
    const vk::Device *device_{};
    const vk::CommandPool *pool_{};
    VkCommandBuffer commandBuffer_{ VK_NULL_HANDLE };
    VkClearValue clearColor_{};
    uint32_t width_{};
    uint32_t height_{};
};

}  // namespace tire::vk