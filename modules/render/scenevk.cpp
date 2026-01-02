
module;

#include <filesystem>

#include <vulkan/vulkan.h>
#include "vulkan/vulkan_core.h"

export module render:scenevk;

import algebra;
import scene;
import context;

import :pipeline;
import :pipeline_vertex_buffer;
import :vertex_buffer;
import :texture_image;

namespace tire {

export struct SceneVK final : tire::Scene {
    SceneVK( const std::filesystem::path &fname, const Context *context, const Pipeline *pipeline )
        : tire::Scene{ fname }
        , context_{ context }
        , pipeline_{ pipeline } {
        //
        try {
            testImage_ =
              std::make_shared<TextureImage>( context_, "/mnt/main/code/assets/textures/PavingStones021_color.tga" );
        } catch ( std::exception &e ) {
            log::error( "Scene === test image {}", e.what() );
        }

        initUploadCommandBuffer();
        initTextureSmpler();

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

    void submit() override {
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

        // Record command to transfer data from CPU to GPU side.
        VkCommandBufferUsageFlags usageFlags = { VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT };

        const VkCommandBufferBeginInfo beginInfo{
          //
          .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
          .flags = usageFlags,
          .pInheritanceInfo = nullptr };

        std::array<VkFence, 1> fences = { uploadFence_ };

        vkResetCommandBuffer( uploadCommandBuffer_, 0 );

        vkBeginCommandBuffer( uploadCommandBuffer_, &beginInfo );

        for ( size_t i{ 0 }; i < nodeListSize; ++i ) {
            VkBufferCopy copyVrt{
              //
              .srcOffset = 0,
              .dstOffset = 0,
              .size = vertBuffersList_[i]->size(),
            };

            vkCmdCopyBuffer(
              uploadCommandBuffer_, vertBuffersList_[i]->stagingBuffer(), vertBuffersList_[i]->deviceBuffer(), 1,
              &copyVrt );

            VkBufferCopy copyNrm{
              //
              .srcOffset = 0,
              .dstOffset = 0,
              .size = nrmlBuffersList_[i]->size(),
            };

            vkCmdCopyBuffer(
              uploadCommandBuffer_, nrmlBuffersList_[i]->stagingBuffer(), nrmlBuffersList_[i]->deviceBuffer(), 1,
              &copyNrm );

            VkBufferCopy copyTxc{
              //
              .srcOffset = 0,
              .dstOffset = 0,
              .size = texcBuffersList_[i]->size(),
            };

            vkCmdCopyBuffer(
              uploadCommandBuffer_, texcBuffersList_[i]->stagingBuffer(), texcBuffersList_[i]->deviceBuffer(), 1,
              &copyTxc );
        }

        vkEndCommandBuffer( uploadCommandBuffer_ );

        std::array<VkPipelineStageFlags, 1> waitStages{
          //
          VK_PIPELINE_STAGE_TRANSFER_BIT,
        };
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

        vkQueueSubmit( context_->graphicsQueue(), 1, &submitInfo, uploadFence_ );

        // NOTE: This fence works but synchroniztion validation error
        // SYNC-HAZARD-WRITE-AFTER-READ still occurs. Conflicting with vkCmdDraw later.
        // Enable VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT in instance
        // to see debug output.
        vkWaitForFences( context_->device(), fences.size(), fences.data(), VK_TRUE, UINT64_MAX );
        vkResetFences( context_->device(), fences.size(), fences.data() );
    }

    void output( const VkCommandBuffer cb ) {
        const auto view = camera().matrix();

        vkCmdBindDescriptorSets(
          cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_->layout(), 0, 1, &textureSet_, 0, nullptr );

        // =================================================================================

        vkCmdBindPipeline( cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_->pipeline() );

        vkCmdPushConstants(
          cb, pipeline_->layout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( algebra::matrix4d ), &view );

        // Record draw commands for all scene objects in command buffer
        for ( size_t object = 0; object < bodyList_.size(); ++object ) {
            const auto c = bodyList_[object]->albedoColor().asVector4f();

            vkCmdPushConstants(
              cb, pipeline_->layout(), VK_SHADER_STAGE_VERTEX_BIT, sizeof( algebra::matrix4d ),
              sizeof( algebra::vector4f ), &c );

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

    auto initUploadCommandBuffer() -> void {
        VkFenceCreateInfo fenceInfo{
          //
          .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
          .pNext = nullptr,
          .flags = 0 };

        vkCreateFence( context_->device(), &fenceInfo, nullptr, &uploadFence_ );

        const VkCommandBufferAllocateInfo allocInfo{
          .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
          .commandPool = context_->commandPool(),
          .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
          .commandBufferCount = 1 };

        vkAllocateCommandBuffers( context_->device(), &allocInfo, &uploadCommandBuffer_ );
    }

    auto initTextureSmpler() -> void {
        const auto info = VkSamplerCreateInfo{
          //
          .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
          .pNext = nullptr,
          .magFilter = VK_FILTER_NEAREST,
          .minFilter = VK_FILTER_NEAREST,
          .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
          .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
          .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        };

        vkCreateSampler( context_->device(), &info, nullptr, &blockySampler_ );

        auto s = dynamic_cast<const PiplineVertexBuffer *>( pipeline_ )->singleTextureSetLayout();
        std::array<VkDescriptorSetLayout, 1> descSetLayouts = { s };
        const auto allocInfo = VkDescriptorSetAllocateInfo{
          //
          .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
          .pNext = nullptr,
          .descriptorPool = context_->descriptorPool(),
          .descriptorSetCount = 1,
          .pSetLayouts = descSetLayouts.data(),
        };

        vkAllocateDescriptorSets( context_->device(), &allocInfo, &textureSet_ );

        //write to the descriptor set so that it points to our empire_diffuse texture
        const auto imageBufferInfo = VkDescriptorImageInfo{
          //
          .sampler = blockySampler_,
          .imageView = testImage_->view(),
          .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        };

        VkWriteDescriptorSet texture1 = {
          //
          .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
          .pNext = nullptr,
          .dstSet = textureSet_,
          .dstBinding = 0,
          .descriptorCount = 1,
          .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
          .pImageInfo = &imageBufferInfo,
        };

        vkUpdateDescriptorSets( context_->device(), 1, &texture1, 0, nullptr );
    }

    void clean() override {
        vkDestroyFence( context_->device(), uploadFence_, nullptr );

        const auto nodeListSize = bodyList_.size();
        for ( size_t i{ 0 }; i < nodeListSize; ++i ) {
            vertBuffersList_[i]->clean();
            nrmlBuffersList_[i]->clean();
            texcBuffersList_[i]->clean();
        }
    };

    void draw() override {};

private:
    const Context *context_;
    const Pipeline *pipeline_;

    std::vector<std::shared_ptr<VertexBuffer>> vertBuffersList_;
    std::vector<std::shared_ptr<VertexBuffer>> nrmlBuffersList_;
    std::vector<std::shared_ptr<VertexBuffer>> texcBuffersList_;

    VkFence uploadFence_{};
    VkCommandBuffer uploadCommandBuffer_{};

    std::shared_ptr<tire::TextureImage> testImage_;
    VkSampler blockySampler_{};
    VkDescriptorSet textureSet_{};
};

}  // namespace tire
