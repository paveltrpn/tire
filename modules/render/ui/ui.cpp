
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

import ui;
import context;
import config;

import :pipeline_ui;
import :vertex_buffer;

namespace tire {

struct UiComponentVisitor final {
    auto operator()( const tire::Label &item ) -> void {
        //
    }

    auto operator()( const tire::Billboard &item ) -> void {
        //
    }
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
            std::visit( UiComponentVisitor{}, item );
        }
    }

    auto draw( const VkCommandBuffer cb ) -> void {
        vkCmdBindPipeline( cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_->pipeline() );

        // vkCmdPushConstants(
        // cb, pipeline_->layout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( algebra::matrix4f ), &viewMatrix );

        vkCmdDraw( cb, 36, 3, 0, 0 );
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