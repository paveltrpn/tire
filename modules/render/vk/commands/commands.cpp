
#include <vulkan/vk_enum_string_helper.h>

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_COMMANDS_CPP{ true };

#include "commands.h"
#include "algebra/matrix4.h"

namespace tire::vk {

// ==========================================================================

struct TransformPushConstants {
    algebra::matrix4f view;
    algebra::matrix4f rtn;
};

struct ViewMatrix {
    algebra::matrix4f view;
};

// ==========================================================================

void RenderFromShader::bind_impl() {
    static float factor{ 0.0f };
    auto projection = algebra::perspective<float>(
        45.0f, static_cast<float>( width_ ) / static_cast<float>( height_ ),
        0.01, 100.0 );
    auto offset = algebra::translate<float>( 0.0f, 0.f, -2.0f );
    offset.transposeSelf();
    TransformPushConstants transform{};
    transform.view = offset * projection;
    transform.rtn = algebra::rotate<float>( factor, 40, 9 );
    factor += 0.3;
    vkCmdPushConstants( commandBuffer_, pipeline_->layout(),
                        VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( transform ),
                        &transform );
    vkCmdDraw( commandBuffer_, verteciesCount_, 3, 0, 0 );

    vkCmdEndRenderPass( commandBuffer_ );

    // NOTE: omit return code check
    vkEndCommandBuffer( commandBuffer_ );
}

// ==========================================================================

void RenderFromBuffer::bind_impl() {
    auto projection = algebra::perspective<float>(
        45.0f, static_cast<float>( width_ ) / static_cast<float>( height_ ),
        0.01, 100.0 );
    auto offset = algebra::translate<float>( 0.0f, 0.f, -10.0f );
    offset.transposeSelf();
    ViewMatrix view{};
    view.view = offset * projection;

    vkCmdPushConstants( commandBuffer_, pipeline_->layout(),
                        VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( view ), &view );

    VkBuffer vertexBuffers[] = { vertexBuffer_ };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers( commandBuffer_, 0, 1, vertexBuffers, offsets );
    vkCmdDraw( commandBuffer_, verteciesCount_, 3, 0, 0 );
    // vkCmdDraw( commandBuffer_, verteciesCount_, 1, 0, 0 );

    vkCmdEndRenderPass( commandBuffer_ );

    // NOTE: omit return code check
    vkEndCommandBuffer( commandBuffer_ );
}

}  // namespace tire::vk
