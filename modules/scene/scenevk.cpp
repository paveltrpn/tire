
module;

#include <filesystem>

#include <vulkan/vulkan.h>
#include "vulkan/vulkan_core.h"

#include "render/context.h"
#include "render/pipelines/pipeline.h"
#include "render/buffers/vertex_buffer.h"
#include "algebra/matrix4.h"

export module scene:scenevk;

import :scene;

namespace tire {

export struct SceneVK final : tire::Scene {
    SceneVK( const std::filesystem::path& fname, const vk::Context* context,
             const vk::Pipeline* pipeline )
        : tire::Scene{ fname }
        , context_{ context }
        , pipeline_{ pipeline } {
        // Create vulkan "vertex buffers"
        const auto nodeListSize = bodyList_.size();
        vertBuffersList_.reserve( nodeListSize );
        for ( size_t i{}; i < nodeListSize; ++i ) {
            auto buf = std::make_shared<vk::VertexBuffer>(
                context_, bodyList_[i]->verteciesCount() );
            vertBuffersList_.push_back( std::move( buf ) );
        }

        // Create vulkan "normal buffers"
        nrmlBuffersList_.reserve( nodeListSize );
        for ( size_t i{}; i < nodeListSize; ++i ) {
            auto buf = std::make_shared<vk::VertexBuffer>(
                context_, bodyList_[i]->verteciesCount() );
            nrmlBuffersList_.push_back( std::move( buf ) );
        }
    }

    void submit() override {
        // Update data in vulkan "vertex" buffers, i.e. copy from CPU memory
        for ( size_t i = 0; auto& buffer : vertBuffersList_ ) {
            buffer->populate( reinterpret_cast<const void*>(
                bodyList_[i]->verteciesData() ) );
            ++i;
        }

        // Update data in vulkan "normal" buffers
        for ( size_t i = 0; auto& buffer : nrmlBuffersList_ ) {
            buffer->populate(
                reinterpret_cast<const void*>( bodyList_[i]->normalsData() ) );
            ++i;
        }
    }

    void draw() override {};

    void clean() override {
        for ( auto& buf : vertBuffersList_ ) {
            buf->clean();
        }

        for ( auto& buf : nrmlBuffersList_ ) {
            buf->clean();
        }
    };

    void output( const VkCommandBuffer cb ) {
        const auto view = camera().matrix();

        vkCmdBindPipeline( cb, VK_PIPELINE_BIND_POINT_GRAPHICS,
                           pipeline_->pipeline() );

        vkCmdPushConstants( cb, pipeline_->layout(), VK_SHADER_STAGE_VERTEX_BIT,
                            0, sizeof( algebra::matrix4d ), &view );

        // Record draw commands for all scene objects in command buffer
        for ( size_t object = 0; object < vertBuffersList_.size(); ++object ) {
            const auto c = bodyList_[object]->albedoColor().asVector4f();
            vkCmdPushConstants(
                cb, pipeline_->layout(), VK_SHADER_STAGE_VERTEX_BIT,
                sizeof( algebra::matrix4d ), sizeof( algebra::vector4f ), &c );

            auto vbo = vertBuffersList_[object]->buffer();
            auto nbo = nrmlBuffersList_[object]->buffer();
            auto vCount = vertBuffersList_[object]->verteciesCount();
            // NOTE: see https://docs.vulkan.org/guide/latest/vertex_input_data_processing.html
            std::array<VkBuffer, 2> vertexBuffers = { vbo, nbo };
            std::array<VkDeviceSize, 2> offsets = { 0, 0 };
            vkCmdBindVertexBuffers( cb, 0, 2, vertexBuffers.data(),
                                    offsets.data() );
            vkCmdDraw( cb, vCount, 3, 0, 0 );
        }
    }

private:
    const vk::Context* context_;
    const vk::Pipeline* pipeline_;

    std::vector<std::shared_ptr<vk::VertexBuffer>> vertBuffersList_;
    std::vector<std::shared_ptr<vk::VertexBuffer>> nrmlBuffersList_;
};

}  // namespace tire
