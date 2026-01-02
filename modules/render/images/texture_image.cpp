
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
        VkDeviceSize imageSize = textureData_.width() * textureData_.height() * textureData_.components();

        if ( textureData_.components() == 4 ) {
            imageFormat_ = VK_FORMAT_R8G8B8A8_SRGB;
        } else if ( textureData_.components() == 3 ) {
            imageFormat_ = VK_FORMAT_R8G8B8_SRGB;
        }

        initStagingBuffer( imageSize );
        uploadToStaging( textureData_.data(), imageSize );
        initDeviceImage( imageSize );
    }

    auto operator=( const TextureImage &other ) -> TextureImage & = delete;
    auto operator=( TextureImage &&other ) -> TextureImage & = delete;

    ~TextureImage() {
        //
        clean();
    };

    auto clean() -> void {
        //
        vmaDestroyImage( context_->allocator(), deviceImage_, deviceAllocation_ );
        vmaDestroyBuffer( context_->allocator(), stagingBuffer_, stagingAllocation_ );
    }

private:
    auto initStagingBuffer( VkDeviceSize size ) -> void {
        VkBufferCreateInfo stagingBufferInfo{
          //
          .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
          .pNext = nullptr,
          .size = size,
          .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        };

        VmaAllocationCreateInfo vmaallocInfo{
          //
          .usage = VMA_MEMORY_USAGE_CPU_ONLY,
        };

        vmaCreateBuffer(
          context_->allocator(), &stagingBufferInfo, &vmaallocInfo, &stagingBuffer_, &stagingAllocation_, nullptr );
    }

    auto uploadToStaging( const void *data, VkDeviceSize size ) -> void {
        void *mappedPtr{};
        vmaMapMemory( context_->allocator(), stagingAllocation_, &mappedPtr );
        memcpy( mappedPtr, data, size );
        vmaUnmapMemory( context_->allocator(), stagingAllocation_ );
    }

    auto initDeviceImage( VkDeviceSize size ) -> void {
        VkExtent3D imageExtent{
          //
          .width = static_cast<uint32_t>( textureData_.width() ),
          .height = static_cast<uint32_t>( textureData_.height() ),
          .depth = 1,
        };

        VkImageCreateInfo imgCreateInfo{
          //
          .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
          .format = imageFormat_,
          .extent = imageExtent,
          .usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        };

        VmaAllocationCreateInfo allocCreateInfo = {
          .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
          .usage = VMA_MEMORY_USAGE_GPU_ONLY,
        };

        vmaCreateImage(
          context_->allocator(), &imgCreateInfo, &allocCreateInfo, &deviceImage_, &deviceAllocation_, nullptr );
    }

private:
    const Context *context_{};

    tire::Tga textureData_;

    VkFormat imageFormat_{};

    VkImage deviceImage_{ VK_NULL_HANDLE };
    VmaAllocation deviceAllocation_{ VK_NULL_HANDLE };

    VkBuffer stagingBuffer_{ VK_NULL_HANDLE };
    VmaAllocation stagingAllocation_{ VK_NULL_HANDLE };
};

}  // namespace tire
