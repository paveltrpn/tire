
module;

#include <array>
#include <memory>
#include <filesystem>
#include <variant>
#include <iostream>

#include <vulkan/vulkan.h>
#include "vulkan/vulkan_core.h"
#include <vulkan/vk_enum_string_helper.h>

#include "vma/vk_mem_alloc.h"

export module render:ui;

import algebra;
import ui;
import context;
import config;
import image;

import :pipeline_ui;
import :vertex_buffer;
import :texture_image;

namespace tire {

using namespace algebra;

#define VERTICIES_PER_QUAD 6
#define OUTPUT_QUADS_COUNT 128

struct QuadDrawBuffer final {
    QuadDrawBuffer( const Context *context, size_t quadsCount )
        : context_{ context }
        , vBuf_{ VertexBuffer{ context_, quadsCount * VERTICIES_PER_QUAD * 3 * sizeof( float ) } }
        , tBuf_{ VertexBuffer{ context_, quadsCount * VERTICIES_PER_QUAD * 2 * sizeof( float ) } }
        , cBuf_{ VertexBuffer{ context_, quadsCount * VERTICIES_PER_QUAD * 4 * sizeof( float ) } } {}

    const Context *context_;

    VertexBuffer vBuf_;
    VertexBuffer tBuf_;
    VertexBuffer cBuf_;

    uint32_t primitievsCount_{};
};

struct UiComponentVisitor final {
    UiComponentVisitor( VkCommandBuffer cb, QuadDrawBuffer &labelBuffer, QuadDrawBuffer &billboardBuffer )
        : cb_{ cb }
        , labelBuffer_{ labelBuffer }
        , billboardBuffer_{ billboardBuffer } {}

    auto operator()( const tire::Label &item ) -> void {
        //
        dispath( item, labelBuffer_ );
    }

    auto operator()( const tire::Billboard &item ) -> void {
        //
        dispath( item, billboardBuffer_ );
    }

    template <typename T>
    auto dispath( const T &item, QuadDrawBuffer &buffer ) -> void {
        const auto vOffset = buffer.primitievsCount_ * 3 * sizeof( float );
        const auto tOffset = buffer.primitievsCount_ * 2 * sizeof( float );
        const auto cOffset = buffer.primitievsCount_ * 4 * sizeof( float );

        const auto vDataPtr = reinterpret_cast<const void *>( item.verteciesData() );
        buffer.vBuf_.memcpy( vDataPtr, item.bufferVerticesSize(), vOffset );

        const auto tDataPtr = reinterpret_cast<const void *>( item.texcrdsData() );
        buffer.tBuf_.memcpy( tDataPtr, item.bufferTexcrdsSize(), tOffset );

        const auto cDataPtr = reinterpret_cast<const void *>( item.clrsData() );
        buffer.cBuf_.memcpy( cDataPtr, item.bufferVertclrsSize(), cOffset );

        VkBufferCopy copyVrt{
          //
          .srcOffset = vOffset,
          .dstOffset = vOffset,
          .size = item.bufferVerticesSize(),
        };

        vkCmdCopyBuffer( cb_, buffer.vBuf_.stagingBuffer(), buffer.vBuf_.deviceBuffer(), 1, &copyVrt );

        VkBufferCopy copyTxc{
          //
          .srcOffset = tOffset,
          .dstOffset = tOffset,
          .size = item.bufferTexcrdsSize(),
        };

        vkCmdCopyBuffer( cb_, buffer.tBuf_.stagingBuffer(), buffer.tBuf_.deviceBuffer(), 1, &copyTxc );

        VkBufferCopy copyClrs{
          //
          .srcOffset = cOffset,
          .dstOffset = cOffset,
          .size = item.bufferVertclrsSize(),
        };

        vkCmdCopyBuffer( cb_, buffer.cBuf_.stagingBuffer(), buffer.cBuf_.deviceBuffer(), 1, &copyClrs );

        buffer.primitievsCount_ += item.lettersCount() * VERTICIES_PER_QUAD;
    }

    VkCommandBuffer cb_;

    QuadDrawBuffer &labelBuffer_;
    QuadDrawBuffer &billboardBuffer_;
};

export struct UiVK final : tire::Ui {
    UiVK( const Context *context )
        : context_{ context } {
        //
        const auto configHandle = Config::instance();
        const auto basePath = configHandle->getBasePath().string();
        const auto fontFile = configHandle->get<std::string>( "ui_font" );

        try {
            testImage_ = std::make_shared<TextureImage>( context_, basePath + "/assets/img_fonts/" + fontFile );
        } catch ( std::exception &e ) {
            log::fatal()( "font image {}", e.what() );
        }

        pipeline_ = std::make_shared<PipelineUi>( context_ );

        auto program = Program{ context_ };
        program.fill( {
          //
          basePath + "/shaders/spirv/vk_ui_VERTEX.spv",
          basePath + "/shaders/spirv/vk_ui_FRAGMENT.spv",
        } );

        pipeline_->buildPipeline( program );

        initTextureSmpler();
        initDescriptorSets();
    }

    auto upload( const VkCommandBuffer cb ) -> void {
        //
        for ( auto &&item : componentsList_ ) {
            std::visit( UiComponentVisitor{ cb, labelBuffer_, billboardBuffer_ }, item );
        }
    }

    auto draw( const VkCommandBuffer cb ) -> void {
        vkCmdBindPipeline( cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_->pipeline() );

        // =================================================================================

        std::array<VkDescriptorSet, 1> setsToBind{ fontDescSet_ };
        vkCmdBindDescriptorSets(
          cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_->layout(), 0, 1, setsToBind.data(), 0, nullptr );

        // =================================================================================

        // Pass viewport size.
        const auto v = std::array<float, 4>{ 48, 48, 48, 48 };
        vkCmdPushConstants( cb, pipeline_->layout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( float ) * 4, &v );

        {
            // Pass disable texture flag.
            const auto f = std::array<uint32_t, 4>{ 0, 0, 0, 0 };
            vkCmdPushConstants(
              cb, pipeline_->layout(), VK_SHADER_STAGE_FRAGMENT_BIT, sizeof( float ) * 4, sizeof( uint32_t ) * 4, &f );

            auto vbo = billboardBuffer_.vBuf_.deviceBuffer();
            auto tbo = billboardBuffer_.tBuf_.deviceBuffer();
            auto cbo = billboardBuffer_.cBuf_.deviceBuffer();

            std::array<VkBuffer, 3> vertexBuffers = { vbo, tbo, cbo };
            std::array<VkDeviceSize, 3> offsets = { 0, 0, 0 };

            vkCmdBindVertexBuffers( cb, 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data() );

            vkCmdDraw( cb, billboardBuffer_.primitievsCount_, 3, 0, 0 );
        }

        {
            // Pass enable texture flag.
            const auto f = std::array<uint32_t, 4>{ 1, 0, 0, 0 };
            vkCmdPushConstants(
              cb, pipeline_->layout(), VK_SHADER_STAGE_FRAGMENT_BIT, sizeof( float ) * 4, sizeof( uint32_t ) * 4, &f );

            auto vbo = labelBuffer_.vBuf_.deviceBuffer();
            auto tbo = labelBuffer_.tBuf_.deviceBuffer();
            auto cbo = labelBuffer_.cBuf_.deviceBuffer();

            std::array<VkBuffer, 3> vertexBuffers = { vbo, tbo, cbo };
            std::array<VkDeviceSize, 3> offsets = { 0, 0, 0 };

            vkCmdBindVertexBuffers( cb, 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data() );

            vkCmdDraw( cb, labelBuffer_.primitievsCount_, 3, 0, 0 );
        }
    }

    auto flush() -> void override {
        //
        labelBuffer_.primitievsCount_ = 0;
        billboardBuffer_.primitievsCount_ = 0;
        componentsList_.clear();
    }

private:
    void initDescriptorSets() {
        auto pipelineDescSetLayouts = pipeline_->pipelineSescSetsLayout();

        // Write to the texture descriptor set.
        auto texDescSetLayout = pipelineDescSetLayouts[0];
        const auto texallocInfo = VkDescriptorSetAllocateInfo{
          //
          .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
          .pNext = nullptr,
          .descriptorPool = context_->descriptorPool(),
          .descriptorSetCount = 1,
          .pSetLayouts = &texDescSetLayout,
        };

        {
            const auto err = vkAllocateDescriptorSets( context_->device(), &texallocInfo, &fontDescSet_ );
            if ( err != VK_SUCCESS ) {
                log::fatal()( "error while allocating descriptorSets {}", string_VkResult( err ) );
            }
        }

        const auto textureImageDescInfo = VkDescriptorImageInfo{
          //
          .sampler = fontSampler_,
          .imageView = testImage_->view(),
          .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        };

        VkWriteDescriptorSet textureWrite = {
          //
          .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
          .pNext = nullptr,
          .dstSet = fontDescSet_,
          .dstBinding = 0,
          .descriptorCount = 1,
          .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
          .pImageInfo = &textureImageDescInfo,
          .pBufferInfo = nullptr,
        };

        vkUpdateDescriptorSets( context_->device(), 1, &textureWrite, 0, nullptr );
    }

    auto initTextureSmpler() -> void {
        const auto info = VkSamplerCreateInfo{
          //
          .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
          .pNext = nullptr,
          .magFilter = VK_FILTER_LINEAR,
          .minFilter = VK_FILTER_LINEAR,
          .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
          .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
          .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
          .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
          .mipLodBias = 0.0f,
          .minLod = 0.0f,
          .maxLod = VK_LOD_CLAMP_NONE,
        };

        vkCreateSampler( context_->device(), &info, nullptr, &fontSampler_ );
    }

private:
    const Context *context_;
    std::shared_ptr<PipelineUi> pipeline_{};

    QuadDrawBuffer labelBuffer_{ context_, OUTPUT_QUADS_COUNT };
    QuadDrawBuffer billboardBuffer_{ context_, OUTPUT_QUADS_COUNT };

    std::shared_ptr<tire::TextureImage> testImage_;
    VkSampler fontSampler_{};
    VkDescriptorSet fontDescSet_{};
};

}  // namespace tire