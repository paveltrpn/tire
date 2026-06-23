#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "vma/vk_mem_alloc.h"

#include "context/context.h"
#include "log/log.h"
#include "image/image.h"
#include "image/tga.h"

namespace tire {

struct TextureImage final {
    TextureImage() = delete;
    TextureImage( const TextureImage &other ) = delete;
    TextureImage( TextureImage &&other ) = delete;

    TextureImage( const Context *context, const std::string &fname );

    auto operator=( const TextureImage &other ) -> TextureImage & = delete;
    auto operator=( TextureImage &&other ) -> TextureImage & = delete;

    [[nodiscard]]
    auto view() const -> VkImageView ;

    ~TextureImage() ;

    auto clean() -> void ;

private:
    auto initStagingBuffer( VkDeviceSize size ) -> void ;

    auto uploadToStaging( const void *data, VkDeviceSize size ) -> void ;
    auto initDeviceImage( VkDeviceSize size ) -> void ;
    auto uploadCmd() -> void ;

    auto initImageView() -> void ;
    auto generateMipmaps( VkImage image, uint32_t texWidth, uint32_t texHeight ) -> void ;

    [[nodiscard]]
    auto mipLevels() const -> uint32_t;

private:
    const Context *context_{};

    uint32_t mipLevels_{ 8 };
    VkFormat imageFormat_{};
    VkExtent3D imageExtent_{};

    VkImage deviceImage_{ VK_NULL_HANDLE };
    VmaAllocation deviceAllocation_{ VK_NULL_HANDLE };

    VkBuffer stagingBuffer_{ VK_NULL_HANDLE };
    VmaAllocation stagingAllocation_{ VK_NULL_HANDLE };

    VkImageView imageView_{};
};  // namespace tire

}  // namespace tire
