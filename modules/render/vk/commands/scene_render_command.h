
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
    SceneRenderCommand( const vk::Device *device, const vk::Pipeline *pipeline,
                        const CommandPool *pool, uint32_t commandsCount );

    SceneRenderCommand( const SceneRenderCommand &other ) = delete;
    SceneRenderCommand( SceneRenderCommand &&other ) = delete;
    SceneRenderCommand &operator=( const SceneRenderCommand &other ) = delete;
    SceneRenderCommand &operator=( SceneRenderCommand &&other ) = delete;

    ~SceneRenderCommand() { clean(); }

    void clean();
    void reset();
    void prepare( VkFramebuffer framebuffer, algebra::matrix4f view );
    void bindBuffer( algebra::vector3f color, VkBuffer vbo, uint32_t vCount );
    void submit( VkSemaphore waitSemaphores, VkSemaphore signalSemaphores,
                 VkFence fence );

protected:
    // Propertis set on "command" creation and stay same trough lifetime
    const vk::Device *device_{};
    const vk::Pipeline *pipeline_{};
    const vk::CommandPool *pool_{};
    VkCommandBuffer command_{ VK_NULL_HANDLE };
    VkClearValue clearColor_{};
    uint32_t width_{};
    uint32_t height_{};
};

}  // namespace tire::vk