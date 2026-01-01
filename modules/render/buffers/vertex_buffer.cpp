
module;

#include <cstring>
#include <format>

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

#include "vma/vk_mem_alloc.h"

export module render:vertex_buffer;

import context;

namespace tire {

struct VertexBuffer final {
    using value_type = float;

    VertexBuffer() = delete;
    VertexBuffer( const VertexBuffer &other ) = delete;
    VertexBuffer( VertexBuffer &&other ) = delete;

    VertexBuffer( const Context *context, size_t size )
        : context_{ context }
        , size_{ size } {
        //
        initStagingBuffer( size );
        initDeviceBuffer( size );
        initUploadCommandBuffer();
    }

    auto operator=( const VertexBuffer &other ) -> VertexBuffer & = delete;
    auto operator=( VertexBuffer &&other ) -> VertexBuffer & = delete;

    ~VertexBuffer() {
        //
        clean();
    };

    auto buffer() -> VkBuffer {
        //
        return deviceBuffer_;
    }

    auto populate( const void *data ) -> void {
        void *mappedPtr{};
        vmaMapMemory( context_->allocator(), stagingAllocation_, &mappedPtr );
        memcpy( mappedPtr, data, size_ );
        vmaUnmapMemory( context_->allocator(), stagingAllocation_ );
    }

    auto submit() -> void {
        VkCommandBufferUsageFlags usageFlags = { VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT };

        const VkCommandBufferBeginInfo beginInfo{
          .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, .flags = usageFlags, .pInheritanceInfo = nullptr };

        vkBeginCommandBuffer( uploadCommandBuffer_, &beginInfo );

        // UPLOAD ITSELF
        VkBufferCopy copy;
        copy.dstOffset = 0;
        copy.srcOffset = 0;
        copy.size = size_;
        vkCmdCopyBuffer( uploadCommandBuffer_, stagingBuffer_, deviceBuffer_, 1, &copy );

        vkEndCommandBuffer( uploadCommandBuffer_ );

        std::array<VkPipelineStageFlags, 0> waitStages = {};
        std::array<VkSemaphore, 0> waitsems{};
        std::array<VkSemaphore, 0> sgnlsems{};
        std::array<VkCommandBuffer, 1> commands{ uploadCommandBuffer_ };

        const VkSubmitInfo submitInfo{
          .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
          .waitSemaphoreCount = waitsems.size(),
          .pWaitSemaphores = waitsems.data(),
          .pWaitDstStageMask = waitStages.data(),
          .commandBufferCount = static_cast<uint32_t>( commands.size() ),
          .pCommandBuffers = commands.data(),
          .signalSemaphoreCount = sgnlsems.size(),
          .pSignalSemaphores = sgnlsems.data() };

        vkQueueSubmit( context_->graphicsQueue(), 1, &submitInfo, uploadFence_ );

        vkWaitForFences( context_->device(), 1, &uploadFence_, true, 9999999999 );
        vkResetFences( context_->device(), 1, &uploadFence_ );

        vkResetCommandBuffer( uploadCommandBuffer_, 0 );
    }

    [[nodiscard]]
    auto size() -> size_t {
        //
        return size_;
    }

    auto clean() -> void {
        //
        vkDestroyFence( context_->device(), uploadFence_, nullptr );
        vmaDestroyBuffer( context_->allocator(), deviceBuffer_, deviceAllocation_ );
        vmaDestroyBuffer( context_->allocator(), stagingBuffer_, stagingAllocation_ );
    }

private:
    auto initStagingBuffer( size_t size ) -> void {
        //Allocate staging buffer.
        VkBufferCreateInfo stagingBufferInfo{};
        stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        stagingBufferInfo.pNext = nullptr;

        stagingBufferInfo.size = size;
        stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

        //Let the VMA library know that this data should be on CPU RAM.
        VmaAllocationCreateInfo vmaallocInfo{};
        vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

        //allocate the buffer
        vmaCreateBuffer(
          context_->allocator(), &stagingBufferInfo, &vmaallocInfo, &stagingBuffer_, &stagingAllocation_, nullptr );
    }

    auto initDeviceBuffer( size_t size ) -> void {
        VkBufferCreateInfo bufCreateInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
        bufCreateInfo.size = size_;
        bufCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

        VmaAllocationCreateInfo allocCreateInfo = {};
        // allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
        allocCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        allocCreateInfo.flags =
          VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;

        vmaCreateBuffer(
          context_->allocator(), &bufCreateInfo, &allocCreateInfo, &deviceBuffer_, &deviceAllocation_, nullptr );
    }

    auto initUploadCommandBuffer() -> void {
        VkFenceCreateInfo fenceInfo{
          .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .pNext = nullptr, .flags = VK_FENCE_CREATE_SIGNALED_BIT };

        vkCreateFence( context_->device(), &fenceInfo, nullptr, &uploadFence_ );

        const VkCommandBufferAllocateInfo allocInfo{
          .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
          .commandPool = context_->commandPool(),
          .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
          .commandBufferCount = 1 };

        vkAllocateCommandBuffers( context_->device(), &allocInfo, &uploadCommandBuffer_ );
    }

private:
    const Context *context_{};
    size_t size_{};

    VkFence uploadFence_;
    VkCommandBuffer uploadCommandBuffer_;

    VkBuffer deviceBuffer_{ VK_NULL_HANDLE };
    VmaAllocation deviceAllocation_{ VK_NULL_HANDLE };

    VkBuffer stagingBuffer_{ VK_NULL_HANDLE };
    VmaAllocation stagingAllocation_{ VK_NULL_HANDLE };
};

}  // namespace tire
