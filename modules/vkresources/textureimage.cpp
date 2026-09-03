module;

#include <cstring>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "vma/vk_mem_alloc.h"

#include "context/context.h"
#include "log/log.h"
#include "image/image.h"
#include "image/tga.h"

export module vkresources : textureimage;

namespace tire {

export struct TextureImage final {
    TextureImage() = delete;
    TextureImage( const TextureImage& other ) = delete;
    TextureImage( TextureImage&& other ) = delete;

    TextureImage( const std::string& fname )
        : _imageFormat{ VK_FORMAT_R8G8B8A8_SRGB } {
        //
        tire::Tga textureData{ fname };

        VkDeviceSize imageSize = textureData.width() * textureData.height() * textureData.components();

        _imageExtent = VkExtent3D{
            //
            .width = static_cast<uint32_t>( textureData.width() ),
            .height = static_cast<uint32_t>( textureData.height() ),
            .depth = 1,
        };

        initStagingBuffer( imageSize );
        initDeviceImage( imageSize );
        uploadToStaging( textureData.data(), imageSize );
        uploadCmd();
        generateMipmaps( _deviceImage, _imageExtent.width, _imageExtent.height );
        initImageView();
    }

    auto operator=( const TextureImage& other ) -> TextureImage& = delete;
    auto operator=( TextureImage&& other ) -> TextureImage& = delete;

    [[nodiscard]]
    auto view() const -> VkImageView {
        //
        return _imageView;
    }

    ~TextureImage() {
        //
        clean();
    };

    auto clean() -> void {
        //
        vmaDestroyImage( Context::instance().allocator(), _deviceImage, _deviceAllocation );
        vmaDestroyBuffer( Context::instance().allocator(), _stagingBuffer, _stagingAllocation );
        vkDestroyImageView( Context::instance().device(), _imageView, nullptr );
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
            const auto err = vmaCreateBuffer( Context::instance().allocator(), &stagingBufferInfo, &vmaallocInfo,
                                              &_stagingBuffer, &_stagingAllocation, nullptr );
            if ( err != VK_SUCCESS ) {
                log::fatal()( "error while creating staging buffer {}", string_VkResult( err ) );
            }
        }
    }

    auto uploadToStaging( const void* data, VkDeviceSize size ) -> void {
        void* mappedPtr{};
        vmaMapMemory( Context::instance().allocator(), _stagingAllocation, &mappedPtr );
        std::memcpy( mappedPtr, data, size );
        vmaUnmapMemory( Context::instance().allocator(), _stagingAllocation );
    }

    auto initDeviceImage( VkDeviceSize size ) -> void {
        const auto imgCreateInfo = VkImageCreateInfo{
            //
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .flags = 0,  // Optional
            .imageType = VK_IMAGE_TYPE_2D,
            .format = _imageFormat,
            .extent = _imageExtent,
            .mipLevels = _mipLevels,
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
            const auto err = vmaCreateImage( Context::instance().allocator(), &imgCreateInfo, &allocCreateInfo,
                                             &_deviceImage, &_deviceAllocation, nullptr );
            if ( err != VK_SUCCESS ) {
                log::fatal()( "error while creating device image {}", string_VkResult( err ) );
            }
        }
    }

    auto uploadCmd() -> void {
        auto c = Context::instance().immediateCommand();

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
            .image = _deviceImage,
            .subresourceRange = range,
        };

        // Barrier the image into the transfer-receive layout.
        vkCmdPipelineBarrier( c.buf(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr,
                              0, nullptr, 1, &imageTransferBarrier );

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
            .imageExtent = _imageExtent,
        };

        // Copy the buffer into the image.
        vkCmdCopyBufferToImage( c.buf(), _stagingBuffer, _deviceImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
                                &copyRegion );

        VkImageMemoryBarrier imageReadableBarrier = imageTransferBarrier;

        imageReadableBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        imageReadableBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageReadableBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        imageReadableBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        // Barrier the image into the shader readable layout.
        vkCmdPipelineBarrier( c.buf(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
                              nullptr, 0, nullptr, 1, &imageReadableBarrier );
    }

    auto initImageView() -> void {
        const auto subResRange = VkImageSubresourceRange{
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = _mipLevels,
            .baseArrayLayer = 0,
            .layerCount = 1,

        };

        VkImageViewCreateInfo imageinfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = _deviceImage,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = VK_FORMAT_R8G8B8A8_SRGB,
            .components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B,
                            VK_COMPONENT_SWIZZLE_A },
            .subresourceRange = subResRange,
        };

        {
            const auto err = vkCreateImageView( Context::instance().device(), &imageinfo, nullptr, &_imageView );
            if ( err != VK_SUCCESS ) {
                log::fatal()( "error while create image view {}", string_VkResult( err ) );
            }
        }
    }

    auto generateMipmaps( VkImage image, uint32_t texWidth, uint32_t texHeight ) -> void {
        auto c = Context::instance().immediateCommand();

        const auto subResource = VkImageSubresourceRange{
            //
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
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

        auto mipWidth = static_cast<int32_t>( texWidth );
        auto mipHeight = static_cast<int32_t>( texHeight );

        for ( uint32_t i = 1; i < _mipLevels; i++ ) {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier( c.buf(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0,
                                  nullptr, 0, nullptr, 1, &barrier );

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

            vkCmdBlitImage( c.buf(), image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image,
                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR );

            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier( c.buf(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
                                  nullptr, 0, nullptr, 1, &barrier );

            if ( mipWidth > 1 ) mipWidth /= 2;
            if ( mipHeight > 1 ) mipHeight /= 2;
        }

        barrier.subresourceRange.baseMipLevel = _mipLevels - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier( c.buf(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
                              nullptr, 0, nullptr, 1, &barrier );
    }

    [[nodiscard]]
    auto mipLevels() const -> uint32_t {
        //
        return _mipLevels;
    }

private:
    uint32_t _mipLevels{ 8 };
    VkFormat _imageFormat{};
    VkExtent3D _imageExtent{};

    VkImage _deviceImage{ VK_NULL_HANDLE };
    VmaAllocation _deviceAllocation{ VK_NULL_HANDLE };

    VkBuffer _stagingBuffer{ VK_NULL_HANDLE };
    VmaAllocation _stagingAllocation{ VK_NULL_HANDLE };

    VkImageView _imageView{};
};  // namespace tire

}  // namespace tire
