
module;

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "vma/vk_mem_alloc.h"

export module render:texture_image;

import image;
import log;

import context;

namespace tire {

export struct TextureImage final {
    TextureImage() = delete;
    TextureImage( const TextureImage &other ) = delete;
    TextureImage( TextureImage &&other ) = delete;

    TextureImage( const Context *context, const std::string &fname )
        : context_{ context }
        , textureData_{ fname } {
        //
        VkDeviceSize imageSize = textureData_.width() * textureData_.height() * textureData_.bpp() / 8;

        VkFormat imageFormat = VK_FORMAT_R8G8B8A8_SRGB;

        initStagingBuffer( imageSize );
        uploadToStaging( textureData_.data(), imageSize );
    }

    auto operator=( const TextureImage &other ) -> TextureImage & = delete;
    auto operator=( TextureImage &&other ) -> TextureImage & = delete;

    ~TextureImage() {
        //
        clean();
    };

    auto clean() -> void {
        //
        vmaDestroyBuffer( context_->allocator(), deviceBuffer_, deviceAllocation_ );
        vmaDestroyBuffer( context_->allocator(), stagingBuffer_, stagingAllocation_ );
    }

private:
    auto initStagingBuffer( VkDeviceSize size ) -> void {
        VkBufferCreateInfo stagingBufferInfo{};
        stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        stagingBufferInfo.pNext = nullptr;

        stagingBufferInfo.size = size;
        stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

        VmaAllocationCreateInfo vmaallocInfo{};
        vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

        vmaCreateBuffer(
          context_->allocator(), &stagingBufferInfo, &vmaallocInfo, &stagingBuffer_, &stagingAllocation_, nullptr );
    }

    auto uploadToStaging( const void *data, VkDeviceSize size ) -> void {
        void *mappedPtr{};
        vmaMapMemory( context_->allocator(), stagingAllocation_, &mappedPtr );
        memcpy( mappedPtr, data, size );
        vmaUnmapMemory( context_->allocator(), stagingAllocation_ );
    }

    auto initDeviceBuffer( VkDeviceSize size ) -> void {
        VkBufferCreateInfo bufCreateInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
        bufCreateInfo.size = size;
        bufCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

        VmaAllocationCreateInfo allocCreateInfo = {};
        allocCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        allocCreateInfo.flags =
          VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;

        vmaCreateBuffer(
          context_->allocator(), &bufCreateInfo, &allocCreateInfo, &deviceBuffer_, &deviceAllocation_, nullptr );
    }

private:
    const Context *context_{};

    tire::Tga textureData_;

    VkBuffer deviceBuffer_{ VK_NULL_HANDLE };
    VmaAllocation deviceAllocation_{ VK_NULL_HANDLE };

    VkBuffer stagingBuffer_{ VK_NULL_HANDLE };
    VmaAllocation stagingAllocation_{ VK_NULL_HANDLE };
};

}  // namespace tire
