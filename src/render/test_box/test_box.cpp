
module;

#include <memory>

#include <vulkan/vulkan.h>
#include "vulkan/vulkan_core.h"
#include <vulkan/vk_enum_string_helper.h>

#include "vma/vk_mem_alloc.h"

export module render:test_box;

import algebra;
import context;

import :test_box_shader;
import :pipeline_test_box;

namespace tire {

export struct TestBox final {
    TestBox( const Context *context )
        : context_{ context } {
        //
        pipeline_ = std::make_unique<PiplineTestBox>( context_ );
        auto testBoxProgram = Program{ context_ };
        testBoxProgram.fill(
          { { vk_simple_box_VERTEX, vertex_stage_suffix }, { vk_simple_box_FRAGMENT, fragment_stage_suffix } } );
        pipeline_->buildPipeline( testBoxProgram );
    }

    auto draw( const VkCommandBuffer cb, float duration ) -> void {
        // =================================
        auto offset = algebra::translate( position_ );
        offset.transpose_self();

        const auto [width, height] = context_->currentExtent();
        // NOTE: Choose right projection matrix!!!
        const auto proj = algebra::perspective<float>(
          40.0f, static_cast<float>( width ) / static_cast<float>( height ), 0.1f, 100.0f );
        const auto viewMatrix = offset * proj;
        angle_ += duration * 25.0f;
        algebra::vector3f ax{ 0.0f, 1.0f, 1.0f };
        ax.normalizeSelf();
        const auto modelMatrix = algebra::rotate( ax, angle_ );
        // =================================

        vkCmdBindPipeline( cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_->pipeline() );

        vkCmdPushConstants(
          cb, pipeline_->layout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( algebra::matrix4f ), &viewMatrix );

        vkCmdPushConstants(
          cb, pipeline_->layout(), VK_SHADER_STAGE_VERTEX_BIT, sizeof( algebra::matrix4f ), sizeof( algebra::matrix4f ),
          &modelMatrix );

        vkCmdDraw( cb, 36, 3, 0, 0 );
    }

    auto setPosition( float x, float y, float z ) -> void {
        //
        position_ = algebra::vector3f{ x, y, z };
    }

    auto setPosition( algebra::vector3f p ) -> void {
        //
        position_ = p;
    }

    auto clean() -> void {
        //
    };

private:
    const Context *context_;
    std::unique_ptr<Pipeline> pipeline_{};

    algebra::vector3f position_{ 0.0, 0.0, 0.0 };

    float angle_{};
};

}  // namespace tire
