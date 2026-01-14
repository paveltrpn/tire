
module;

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

import :pipeline_ui;
import :vertex_buffer;

namespace tire {

using namespace algebra;

struct UiComponentVisitor final {
    UiComponentVisitor( VkCommandBuffer cb, VertexBuffer *vBuf, VertexBuffer *cBuf, VertexBuffer *tBuf )
        : cb_{ cb }
        , vBuf_{ vBuf }
        , tBuf_{ tBuf }
        , cBuf_{ cBuf } {}

    auto operator()( const tire::Label &item ) -> void {
        //
        const auto vDataPtr = reinterpret_cast<const void *>( item.verteciesData() );
        vBuf_->memcpy( vDataPtr, item.bufferVerticesSize() );

        const auto tDataPtr = reinterpret_cast<const void *>( item.texcrdsData() );
        cBuf_->memcpy( tDataPtr, item.bufferTexcrdsSize() );

        const auto cDataPtr = reinterpret_cast<const void *>( item.clrsData() );
        tBuf_->memcpy( tDataPtr, item.bufferVertclrsSize() );

        VkBufferCopy copyVrt{
          //
          .srcOffset = 0,
          .dstOffset = 0,
          .size = vBuf_->size(),
        };

        vkCmdCopyBuffer( cb_, vBuf_->stagingBuffer(), vBuf_->deviceBuffer(), 1, &copyVrt );

        VkBufferCopy copyNrm{
          //
          .srcOffset = 0,
          .dstOffset = 0,
          .size = cBuf_->size(),
        };

        vkCmdCopyBuffer( cb_, cBuf_->stagingBuffer(), cBuf_->deviceBuffer(), 1, &copyNrm );

        VkBufferCopy copyTxc{
          //
          .srcOffset = 0,
          .dstOffset = 0,
          .size = tBuf_->size(),
        };

        vkCmdCopyBuffer( cb_, tBuf_->stagingBuffer(), tBuf_->deviceBuffer(), 1, &copyTxc );
    }

    auto operator()( const tire::Billboard &item ) -> void {
        //
    }

    VkCommandBuffer cb_;

    VertexBuffer *vBuf_;
    VertexBuffer *cBuf_;
    VertexBuffer *tBuf_;
};

export struct UiVK final : tire::Ui {
    UiVK( const Context *context )
        : context_{ context } {
        //

        const auto configHandle = Config::instance();
        const auto basePath = configHandle->getBasePath().string();

        pipeline_ = std::make_shared<PipelineUi>( context_ );

        auto program = Program{ context_ };
        program.fill( {
          //
          basePath + "/shaders/spirv/vk_ui_VERTEX.spv",
          basePath + "/shaders/spirv/vk_ui_FRAGMENT.spv",
        } );

        pipeline_->buildPipeline( program );

#define BUF_SIZE 1024 * 1024
        vBuf_ = std::make_shared<VertexBuffer>( context_, BUF_SIZE );
        cBuf_ = std::make_shared<VertexBuffer>( context_, BUF_SIZE );
        tBuf_ = std::make_shared<VertexBuffer>( context_, BUF_SIZE );
    }

    auto upload( const VkCommandBuffer cb ) -> void {
        //
        for ( auto &&item : componentsList_ ) {
            std::visit( UiComponentVisitor{ cb, vBuf_.get(), tBuf_.get(), cBuf_.get() }, item );
        }
    }

    auto draw( const VkCommandBuffer cb, matrix4f m ) -> void {
        vkCmdBindPipeline( cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_->pipeline() );

        vkCmdPushConstants( cb, pipeline_->layout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( algebra::matrix4f ), &m );

        vkCmdDraw( cb, 9 * 6, 3, 0, 0 );
    }

    auto flush() -> void override {
        //
        submittedPrimitievesCount_ = 0;
        componentsList_.clear();
    }

private:
    const Context *context_;
    std::shared_ptr<PipelineUi> pipeline_{};

    std::shared_ptr<VertexBuffer> vBuf_;
    std::shared_ptr<VertexBuffer> cBuf_;
    std::shared_ptr<VertexBuffer> tBuf_;

    uint32_t submittedPrimitievesCount_{};
};

}  // namespace tire