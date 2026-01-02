
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
        , imageFormat_{ VK_FORMAT_R8G8B8A8_SRGB }
        , textureData_{ fname } {
        //
        VkDeviceSize imageSize = textureData_.width() * textureData_.height() * textureData_.components();

        imageExtent_ = VkExtent3D{
          //
          .width = static_cast<uint32_t>( textureData_.width() ),
          .height = static_cast<uint32_t>( textureData_.height() ),
          .depth = 1,
        };

        initUploadCommandBuffer();
        initStagingBuffer( imageSize );
        initDeviceImage( imageSize );
        uploadToStaging( textureData_.data(), imageSize );
        upload();
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
    auto initUploadCommandBuffer() -> void {
        const auto allocInfo = VkCommandBufferAllocateInfo{
          .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
          .commandPool = context_->commandPool(),
          .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
          .commandBufferCount = 1 };

        vkAllocateCommandBuffers( context_->device(), &allocInfo, &uploadCommandBuffer_ );
    }

    auto initStagingBuffer( VkDeviceSize size ) -> void {
        const auto stagingBufferInfo = VkBufferCreateInfo{
          //
          .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
          .pNext = nullptr,
          .size = size,
          .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        };

        const auto vmaallocInfo = VmaAllocationCreateInfo{
          //
          .usage = VMA_MEMORY_USAGE_CPU_ONLY,
        };

        {
            const auto err = vmaCreateBuffer(
              context_->allocator(), &stagingBufferInfo, &vmaallocInfo, &stagingBuffer_, &stagingAllocation_, nullptr );
            if ( err != VK_SUCCESS ) {
                log::fatal( "TextureImage === error while creating staging buffer {}", string_VkResult( err ) );
            }
        }
    }

    auto uploadToStaging( const void *data, VkDeviceSize size ) -> void {
        void *mappedPtr{};
        vmaMapMemory( context_->allocator(), stagingAllocation_, &mappedPtr );
        memcpy( mappedPtr, data, size );
        vmaUnmapMemory( context_->allocator(), stagingAllocation_ );
    }

    auto initDeviceImage( VkDeviceSize size ) -> void {
        const auto imgCreateInfo = VkImageCreateInfo{
          //
          .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
          .flags = 0,  // Optional
          .imageType = VK_IMAGE_TYPE_2D,
          .format = imageFormat_,
          .extent = imageExtent_,
          .mipLevels = 1,
          .arrayLayers = 1,
          .samples = VK_SAMPLE_COUNT_1_BIT,
          .tiling = VK_IMAGE_TILING_OPTIMAL,
          .usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
          .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
          .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        };

        const auto allocCreateInfo = VmaAllocationCreateInfo{
          .flags = 0,
          .usage = VMA_MEMORY_USAGE_GPU_ONLY,
        };

        {
            const auto err = vmaCreateImage(
              context_->allocator(), &imgCreateInfo, &allocCreateInfo, &deviceImage_, &deviceAllocation_, nullptr );
            if ( err != VK_SUCCESS ) {
                log::fatal( "TextureImage === error while creating device image {}", string_VkResult( err ) );
            }
        }
    }

    auto upload() -> void {
        VkCommandBufferUsageFlags usageFlags = { VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT };

        const VkCommandBufferBeginInfo beginInfo{
          //
          .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
          .flags = usageFlags,
          .pInheritanceInfo = nullptr };

        vkResetCommandBuffer( uploadCommandBuffer_, 0 );

        vkBeginCommandBuffer( uploadCommandBuffer_, &beginInfo );

        const auto range = VkImageSubresourceRange{
          //
          .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
          .baseMipLevel = 0,
          .levelCount = 1,
          .baseArrayLayer = 0,
          .layerCount = 1,
        };

        const auto imageTransferBarrier = VkImageMemoryBarrier{
          //
          .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
          .srcAccessMask = 0,
          .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
          .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
          .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
          .image = deviceImage_,
          .subresourceRange = range,
        };

        //barrier the image into the transfer-receive layout
        vkCmdPipelineBarrier(
          uploadCommandBuffer_, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
          nullptr, 1, &imageTransferBarrier );

        const auto imageSubresource = VkImageSubresourceLayers{
          //
          .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
          .mipLevel = 0,
          .baseArrayLayer = 0,
          .layerCount = 1,

        };

        const auto copyRegion = VkBufferImageCopy{
          //
          .bufferOffset = 0,           .bufferRowLength = 0,
          .bufferImageHeight = 0,      .imageSubresource = imageSubresource,
          .imageExtent = imageExtent_,
        };

        //copy the buffer into the image
        vkCmdCopyBufferToImage(
          uploadCommandBuffer_, stagingBuffer_, deviceImage_, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion );

        VkImageMemoryBarrier imageReadableBarrier = imageTransferBarrier;

        imageReadableBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        imageReadableBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageReadableBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        imageReadableBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        //barrier the image into the shader readable layout
        vkCmdPipelineBarrier(
          uploadCommandBuffer_, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0,
          nullptr, 1, &imageReadableBarrier );

        vkEndCommandBuffer( uploadCommandBuffer_ );

        std::array<VkPipelineStageFlags, 0> waitStages{};
        std::array<VkSemaphore, 0> waitsems{};
        std::array<VkSemaphore, 0> sgnlsems{};
        std::array<VkCommandBuffer, 1> commands{
          //
          uploadCommandBuffer_,
        };

        const VkSubmitInfo submitInfo{
          .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
          .waitSemaphoreCount = waitsems.size(),
          .pWaitSemaphores = waitsems.data(),
          .pWaitDstStageMask = waitStages.data(),
          .commandBufferCount = static_cast<uint32_t>( commands.size() ),
          .pCommandBuffers = commands.data(),
          .signalSemaphoreCount = sgnlsems.size(),
          .pSignalSemaphores = sgnlsems.data() };

        {
            const auto err = vkQueueSubmit( context_->graphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE );
            if ( err != VK_SUCCESS ) {
                log::fatal( "TextureImage === error while submitting upload command {}", string_VkResult( err ) );
            }
        }
    }

private:
    const Context *context_{};

    tire::Tga textureData_;

    VkFormat imageFormat_{};
    VkExtent3D imageExtent_{};

    VkCommandBuffer uploadCommandBuffer_{};

    VkImage deviceImage_{ VK_NULL_HANDLE };
    VmaAllocation deviceAllocation_{ VK_NULL_HANDLE };

    VkBuffer stagingBuffer_{ VK_NULL_HANDLE };
    VmaAllocation stagingAllocation_{ VK_NULL_HANDLE };
};  // namespace tire

}  // namespace tire
