
module;

#include <filesystem>

#include <vulkan/vulkan.h>
#include "vulkan/vulkan_core.h"

#include "context/context.h"

export module render:scenevk;

import algebra;
import scene;

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
    }

    void submit() override {
        const auto nodeListSize = bodyList_.size();

        for ( size_t i{ 0 }; i < nodeListSize; ++i ) {
            // Update data in vulkan "vertex" buffers, i.e. copy from CPU memory.
            const auto vDataPtr = reinterpret_cast<const void *>( bodyList_[i]->verteciesData() );
            vertBuffersList_[i]->populate( vDataPtr );

            // Update data in vulkan "normal" buffers
            const auto nDataPtr = reinterpret_cast<const void *>( bodyList_[i]->normalsData() );
            nrmlBuffersList_[i]->populate( nDataPtr );
        }
    }

    void draw() override {};

    void clean() override {
        const auto nodeListSize = bodyList_.size();
        for ( size_t i{ 0 }; i < nodeListSize; ++i ) {
            vertBuffersList_[i]->clean();
            nrmlBuffersList_[i]->clean();
        }
    };

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

            auto vbo = vertBuffersList_[object]->buffer();
            auto nbo = nrmlBuffersList_[object]->buffer();
            auto vCount = bodyList_[object]->verteciesCount();

            // NOTE: see https://docs.vulkan.org/guide/latest/vertex_input_data_processing.html
            std::array<VkBuffer, 2> vertexBuffers = { vbo, nbo };
            std::array<VkDeviceSize, 2> offsets = { 0, 0 };

            vkCmdBindVertexBuffers( cb, 0, 2, vertexBuffers.data(), offsets.data() );
            vkCmdDraw( cb, vCount, 3, 0, 0 );
        }
    }

private:
    const Context *context_;
    const Pipeline *pipeline_;

    std::vector<std::shared_ptr<VertexBuffer>> vertBuffersList_;
    std::vector<std::shared_ptr<VertexBuffer>> nrmlBuffersList_;
};

}  // namespace tire
