
module;

#include <filesystem>

#include <vulkan/vulkan.h>
#include "vulkan/vulkan_core.h"

export module render:scenevk;

import algebra;
import scene;
import context;

import :pipeline;
import :vertex_buffer;

namespace tire {

export struct SceneVK final : tire::Scene {
    SceneVK( const std::filesystem::path &fname, const Context *context, const Pipeline *pipeline )
        : tire::Scene{ fname }
        , context_{ context }
        , pipeline_{ pipeline } {
        //
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
        }

        initUploadCommandBuffer();
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
            VkBufferCopy copyVrt{ .srcOffset = 0, .dstOffset = 0, .size = vertBuffersList_[i]->size() };
            vkCmdCopyBuffer(
              uploadCommandBuffer_, vertBuffersList_[i]->stagingBuffer(), vertBuffersList_[i]->deviceBuffer(), 1,
              &copyVrt );

            VkBufferCopy copyNrm{ .srcOffset = 0, .dstOffset = 0, .size = nrmlBuffersList_[i]->size() };
            vkCmdCopyBuffer(
              uploadCommandBuffer_, nrmlBuffersList_[i]->stagingBuffer(), nrmlBuffersList_[i]->deviceBuffer(), 1,
              &copyNrm );
        }

        vkEndCommandBuffer( uploadCommandBuffer_ );

        std::array<VkPipelineStageFlags, 1> waitStages = { VK_PIPELINE_STAGE_TRANSFER_BIT };
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

        // NOTE: This fence works but synchroniztion validation error
        // SYNC-HAZARD-WRITE-AFTER-READ still occurs. Conflicting with vkCmdDraw later.
        // Enable VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT in instance
        // to see debug output.
        vkWaitForFences( context_->device(), fences.size(), fences.data(), VK_TRUE, UINT64_MAX );
        vkResetFences( context_->device(), fences.size(), fences.data() );
    }

    void output( const VkCommandBuffer cb ) {
        const auto view = camera().matrix();

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
            auto vCount = bodyList_[object]->verteciesCount();

            // NOTE: see https://docs.vulkan.org/guide/latest/vertex_input_data_processing.html
            std::array<VkBuffer, 2> vertexBuffers = { vbo, nbo };
            std::array<VkDeviceSize, 2> offsets = { 0, 0 };

            vkCmdBindVertexBuffers( cb, 0, 2, vertexBuffers.data(), offsets.data() );
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

    void clean() override {
        vkDestroyFence( context_->device(), uploadFence_, nullptr );

        const auto nodeListSize = bodyList_.size();
        for ( size_t i{ 0 }; i < nodeListSize; ++i ) {
            vertBuffersList_[i]->clean();
            nrmlBuffersList_[i]->clean();
        }
    };

    void draw() override {};

private:
    const Context *context_;
    const Pipeline *pipeline_;

    std::vector<std::shared_ptr<VertexBuffer>> vertBuffersList_;
    std::vector<std::shared_ptr<VertexBuffer>> nrmlBuffersList_;

    VkFence uploadFence_{};
    VkCommandBuffer uploadCommandBuffer_{};
};

}  // namespace tire
