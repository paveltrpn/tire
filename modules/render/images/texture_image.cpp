
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

        initStagingBuffer( imageSize );
        initDeviceImage( imageSize );
        uploadToStaging( textureData_.data(), imageSize );
        uploadCmd();
        initImageView();
        generateMipmaps( deviceImage_, imageExtent_.width, imageExtent_.height );
    }

    auto operator=( const TextureImage &other ) -> TextureImage & = delete;
    auto operator=( TextureImage &&other ) -> TextureImage & = delete;

    [[nodiscard]]
    auto view() const -> VkImageView {
        //
        return imageView_;
    }

    ~TextureImage() {
        //
        clean();
    };

    auto clean() -> void {
        //
        vmaDestroyImage( context_->allocator(), deviceImage_, deviceAllocation_ );
        vmaDestroyBuffer( context_->allocator(), stagingBuffer_, stagingAllocation_ );
        vkDestroyImageView( context_->device(), imageView_, nullptr );
    }

private:
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
          .mipLevels = mipLevels_,
          .arrayLayers = 1,
          .samples = VK_SAMPLE_COUNT_1_BIT,
          .tiling = VK_IMAGE_TILING_OPTIMAL,
          .usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
          .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
          .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        };

        const auto allocCreateInfo = VmaAllocationCreateInfo{
          .flags = 0,
          .usage = VMA_MEMORY_USAGE_GPU_ONLY,
          .requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        };

        {
            const auto err = vmaCreateImage(
              context_->allocator(), &imgCreateInfo, &allocCreateInfo, &deviceImage_, &deviceAllocation_, nullptr );
            if ( err != VK_SUCCESS ) {
                log::fatal( "TextureImage === error while creating device image {}", string_VkResult( err ) );
            }
        }
    }

    auto uploadCmd() -> void {
        // Allocate command buffer.
        const auto allocInfo = VkCommandBufferAllocateInfo{
          .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
          .commandPool = context_->commandPool(),
          .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
          .commandBufferCount = 1 };

        vkAllocateCommandBuffers( context_->device(), &allocInfo, &uploadCommandBuffer_ );

        // Record command.
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

    auto initImageView() -> void {
        const auto subResRange = VkImageSubresourceRange{
          .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
          .baseMipLevel = 0,
          .levelCount = 1,
          .baseArrayLayer = 0,
          .layerCount = 1,

        };

        VkImageViewCreateInfo imageinfo = {
          .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
          .image = deviceImage_,
          .viewType = VK_IMAGE_VIEW_TYPE_2D,
          .format = VK_FORMAT_R8G8B8A8_SRGB,
          .subresourceRange = subResRange,
        };

        {
            const auto err = vkCreateImageView( context_->device(), &imageinfo, nullptr, &imageView_ );
            if ( err != VK_SUCCESS ) {
                log::fatal( "TextureImage === error while create image view {}", string_VkResult( err ) );
            }
        }
    }

    auto generateMipmaps( VkImage image, int32_t texWidth, int32_t texHeight ) -> void {
        //VkCommandBuffer commandBuffer = context_->beginSingleCommand();
        auto c = context_->immidiateCommand();

        const auto subResource = VkImageSubresourceRange{
          //
          .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
          .levelCount = 1,
          .baseArrayLayer = 0,
          .layerCount = 1,
        };

        VkImageMemoryBarrier barrier{
          //
          .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
          .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
          .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
          .image = image,
          .subresourceRange = subResource,
        };

        int32_t mipWidth = texWidth;
        int32_t mipHeight = texHeight;

        for ( uint32_t i = 1; i < mipLevels_; i++ ) {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(
              c.buf(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1,
              &barrier );

            VkImageBlit blit{};
            blit.srcOffsets[0] = { 0, 0, 0 };
            blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;
            blit.dstOffsets[0] = { 0, 0, 0 };
            blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            vkCmdBlitImage(
              c.buf(), image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
              &blit, VK_FILTER_LINEAR );

            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(
              c.buf(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr,
              1, &barrier );

            if ( mipWidth > 1 ) mipWidth /= 2;
            if ( mipHeight > 1 ) mipHeight /= 2;
        }

        barrier.subresourceRange.baseMipLevel = mipLevels_ - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(
          c.buf(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1,
          &barrier );

        //context_->endSingleCommand( commandBuffer );
    }

    [[nodiscard]]
    auto mipLevels() const -> uint32_t {
        //
        return mipLevels_;
    }

private:
    const Context *context_{};

    tire::Tga textureData_;

    uint32_t mipLevels_{ 4 };
    VkFormat imageFormat_{};
    VkExtent3D imageExtent_{};

    VkCommandBuffer uploadCommandBuffer_{};

    VkImage deviceImage_{ VK_NULL_HANDLE };
    VmaAllocation deviceAllocation_{ VK_NULL_HANDLE };

    VkBuffer stagingBuffer_{ VK_NULL_HANDLE };
    VmaAllocation stagingAllocation_{ VK_NULL_HANDLE };

    VkImageView imageView_{};
};  // namespace tire

}  // namespace tire
