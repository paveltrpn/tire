
module;

#include <filesystem>

#include <vulkan/vulkan.h>
#include "vulkan/vulkan_core.h"
#include <vulkan/vk_enum_string_helper.h>

#include "vma/vk_mem_alloc.h"

export module render:scenevk;

import algebra;
import scene;
import context;
import config;

import :pipeline_scene;
import :vertex_buffer;
import :texture_image;

namespace tire {

export struct SceneVK final : tire::Scene {
    SceneVK( const std::filesystem::path &fname, const Context *context )
        : tire::Scene{ fname }
        , context_{ context } {
        //
        const auto configHandle = Config::instance();
        const auto basePath = configHandle->getBasePath().string();

        pipeline_ = std::make_unique<PiplineScene>( context_ );
        auto vertexBufferProgram = Program{ context_ };
        vertexBufferProgram.fill(
          { basePath + "/shaders/spirv/vk_vertexBuffer_VERTEX.spv",
            basePath + "/shaders/spirv/vk_vertexBuffer_FRAGMENT.spv" } );
        pipeline_->buildPipeline( vertexBufferProgram );

        try {
            testImage_ =
              std::make_shared<TextureImage>( context_, "/mnt/main/code/assets/textures/PavingStones021_color.tga" );
        } catch ( std::exception &e ) {
            log::fatal()( "test image {}", e.what() );
        }

        initTextureSmpler();
        initOmniLigtBuffer();
        initDescriptorSets();

        const auto nodeListSize = bodyList_.size();

        vertBuffersList_.reserve( nodeListSize );
        nrmlBuffersList_.reserve( nodeListSize );

        for ( size_t i{ 0 }; i < nodeListSize; ++i ) {
            // Create vulkan "vertex buffers".
            auto vBuf = std::make_shared<VertexBuffer>( context_, bodyList_[i]->bufferVerticesSize() );
            vertBuffersList_.push_back( std::move( vBuf ) );

            // Create vulkan "normal buffers".
            auto nBuf = std::make_shared<VertexBuffer>( context_, bodyList_[i]->bufferNormalsSize() );
            nrmlBuffersList_.push_back( std::move( nBuf ) );

            auto tBuf = std::make_shared<VertexBuffer>( context_, bodyList_[i]->bufferTexcrdsSize() );
            texcBuffersList_.push_back( std::move( tBuf ) );
        }
    }

    void upload( const VkCommandBuffer cb ) {
        const auto nodeListSize = bodyList_.size();

        // Fill CPU side buffer (staging) with new verticies data.
        for ( size_t i{ 0 }; i < nodeListSize; ++i ) {
            // Update data in vulkan "vertex" buffers, i.e. copy from CPU memory.
            const auto vDataPtr = reinterpret_cast<const void *>( bodyList_[i]->verteciesData() );
            vertBuffersList_[i]->populate( vDataPtr );

            // Update data in vulkan "normal" buffers
            const auto nDataPtr = reinterpret_cast<const void *>( bodyList_[i]->normalsData() );
            nrmlBuffersList_[i]->populate( nDataPtr );

            const auto tDataPtr = reinterpret_cast<const void *>( bodyList_[i]->texcrdsData() );
            texcBuffersList_[i]->populate( tDataPtr );
        }

        for ( size_t i{ 0 }; i < nodeListSize; ++i ) {
            VkBufferCopy copyVrt{
              //
              .srcOffset = 0,
              .dstOffset = 0,
              .size = vertBuffersList_[i]->size(),
            };

            vkCmdCopyBuffer(
              cb, vertBuffersList_[i]->stagingBuffer(), vertBuffersList_[i]->deviceBuffer(), 1, &copyVrt );

            VkBufferCopy copyNrm{
              //
              .srcOffset = 0,
              .dstOffset = 0,
              .size = nrmlBuffersList_[i]->size(),
            };

            vkCmdCopyBuffer(
              cb, nrmlBuffersList_[i]->stagingBuffer(), nrmlBuffersList_[i]->deviceBuffer(), 1, &copyNrm );

            VkBufferCopy copyTxc{
              //
              .srcOffset = 0,
              .dstOffset = 0,
              .size = texcBuffersList_[i]->size(),
            };

            vkCmdCopyBuffer(
              cb, texcBuffersList_[i]->stagingBuffer(), texcBuffersList_[i]->deviceBuffer(), 1, &copyTxc );
        }
    }

    void draw( const VkCommandBuffer cb ) {
        vkCmdBindPipeline( cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_->pipeline() );

        // =================================================================================

        std::array<VkDescriptorSet, 2> setsToBind{ textureSet_, omniLightSet_ };
        vkCmdBindDescriptorSets(
          cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_->layout(), 0, 2, setsToBind.data(), 0, nullptr );

        // =================================================================================

        const auto view = camera().matrix();
        vkCmdPushConstants(
          cb, pipeline_->layout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( algebra::matrix4d ), &view );

        const auto eye = camera().eye();
        vkCmdPushConstants(
          cb, pipeline_->layout(), VK_SHADER_STAGE_VERTEX_BIT, sizeof( algebra::matrix4d ), sizeof( algebra::vector3d ),
          &eye );

        // Record draw commands for all scene objects into command buffer.
        for ( size_t object = 0; object < bodyList_.size(); ++object ) {
            const auto c = bodyList_[object]->albedoColor().asVector4f();

            auto vbo = vertBuffersList_[object]->deviceBuffer();
            auto nbo = nrmlBuffersList_[object]->deviceBuffer();
            auto tbo = texcBuffersList_[object]->deviceBuffer();
            auto vCount = bodyList_[object]->verteciesCount();

            // NOTE: see https://docs.vulkan.org/guide/latest/vertex_input_data_processing.html
            std::array<VkBuffer, 3> vertexBuffers = { vbo, nbo, tbo };
            std::array<VkDeviceSize, 3> offsets = { 0, 0, 0 };

            vkCmdBindVertexBuffers( cb, 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data() );
            vkCmdDraw( cb, vCount, 3, 0, 0 );
        }
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

        vkCreateSampler( context_->device(), &info, nullptr, &blockySampler_ );
    }

    auto initOmniLigtBuffer() -> void {
        const auto bufferInfo = VkBufferCreateInfo{
          //
          .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
          .pNext = nullptr,
          .size = sizeof( tire::OmniLight ) * lightList_.size(),
          .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        };

        const auto vmaallocInfo = VmaAllocationCreateInfo{
          //
          .usage = VMA_MEMORY_USAGE_CPU_TO_GPU,
        };

        {
            const auto err = vmaCreateBuffer(
              context_->allocator(), &bufferInfo, &vmaallocInfo, &omniLightUniform_, &omniLightAllocation_, nullptr );
            if ( err != VK_SUCCESS ) {
                log::fatal()( "error while creating omni light uniform buffer {}", string_VkResult( err ) );
            }
        }

        void *data{};
        vmaMapMemory( context_->allocator(), omniLightAllocation_, &data );
        memcpy( data, lightList_.data(), sizeof( tire::OmniLight ) * lightList_.size() );
        vmaUnmapMemory( context_->allocator(), omniLightAllocation_ );
    }

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
            const auto err = vkAllocateDescriptorSets( context_->device(), &texallocInfo, &textureSet_ );
            if ( err != VK_SUCCESS ) {
                log::fatal()( "error while allocating descriptorSets {}", string_VkResult( err ) );
            }
        }

        const auto textureImageDescInfo = VkDescriptorImageInfo{
          //
          .sampler = blockySampler_,
          .imageView = testImage_->view(),
          .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        };

        VkWriteDescriptorSet textureWrite = {
          //
          .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
          .pNext = nullptr,
          .dstSet = textureSet_,
          .dstBinding = 0,
          .descriptorCount = 1,
          .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
          .pImageInfo = &textureImageDescInfo,
          .pBufferInfo = nullptr,
        };

        vkUpdateDescriptorSets( context_->device(), 1, &textureWrite, 0, nullptr );

        // Write to the omni light descriptor set.
        auto lightDescSetLayout = pipelineDescSetLayouts[1];
        const auto lightallocInfo = VkDescriptorSetAllocateInfo{
          //
          .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
          .pNext = nullptr,
          .descriptorPool = context_->descriptorPool(),
          .descriptorSetCount = 1,
          .pSetLayouts = &lightDescSetLayout,
        };

        {
            const auto err = vkAllocateDescriptorSets( context_->device(), &lightallocInfo, &omniLightSet_ );
            if ( err != VK_SUCCESS ) {
                log::fatal()( "error while allocating descriptorSets {}", string_VkResult( err ) );
            }
        }

        const auto omniLightBufferDescInfo = VkDescriptorBufferInfo{
          //
          .buffer = omniLightUniform_,
          .offset = 0,
          .range = sizeof( tire::OmniLight ) * lightList_.size(),
        };

        VkWriteDescriptorSet omniLightWrite = {
          //
          .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
          .pNext = nullptr,
          .dstSet = omniLightSet_,
          .dstBinding = 0,
          .descriptorCount = 1,
          .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
          .pImageInfo = nullptr,
          .pBufferInfo = &omniLightBufferDescInfo,
        };

        vkUpdateDescriptorSets( context_->device(), 1, &omniLightWrite, 0, nullptr );
    }

    void clean() override {
        vmaDestroyBuffer( context_->allocator(), omniLightUniform_, omniLightAllocation_ );

        const auto nodeListSize = bodyList_.size();
        for ( size_t i{ 0 }; i < nodeListSize; ++i ) {
            vertBuffersList_[i]->clean();
            nrmlBuffersList_[i]->clean();
            texcBuffersList_[i]->clean();
        }
    };

private:
    const Context *context_;
    std::unique_ptr<PiplineScene> pipeline_{};

    std::vector<std::shared_ptr<VertexBuffer>> vertBuffersList_;
    std::vector<std::shared_ptr<VertexBuffer>> nrmlBuffersList_;
    std::vector<std::shared_ptr<VertexBuffer>> texcBuffersList_;

    std::shared_ptr<tire::TextureImage> testImage_;
    VkSampler blockySampler_{};
    VkDescriptorSet textureSet_{};

    VkBuffer omniLightUniform_{};
    VmaAllocation omniLightAllocation_{};
    VkDescriptorSet omniLightSet_{};
};

}  // namespace tire
