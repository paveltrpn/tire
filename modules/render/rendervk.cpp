
#include "pipelines/program.h"

#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include "config/config.h"
#include "pipelines/pipeline.h"
#include "pipelines/pipeline_vertex_buffer.h"
#include "pipelines/pipeline_test_box.h"
#include "pipelines/test_box_shader.h"
#include "rendervk.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_RENDERVK_CPP{ true };

namespace tire {

auto RenderVK::init( vk::Context* context ) -> void {
    context_ = context;
    try {
        const auto configHandle = Config::instance();
        const auto basePath = configHandle->getBasePath().string();

        // =============================================================
        piplineVertexBuffer_ =
            std::make_unique<vk::PiplineVertexBuffer>( context_ );
        auto vertexBufferProgram = vk::Program{ context_ };
        vertexBufferProgram.fill(
            { basePath + "/assets/shaders/spirv/vk_vertexBuffer_VERTEX.spv",
              basePath +
                  "/assets/shaders/spirv/vk_vertexBuffer_FRAGMENT.spv" } );
        piplineVertexBuffer_->buildPipeline( vertexBufferProgram );

        // =============================================================
        piplineTestBox_ = std::make_unique<vk::PiplineTestBox>( context_ );
        auto testBoxProgram = vk::Program{ context_ };
        testBoxProgram.fill(
            { { vk::vk_simple_box_VERTEX, vk::vertex_stage_suffix },
              { vk::vk_simple_box_FRAGMENT, vk::fragment_stage_suffix } } );
        piplineTestBox_->buildPipeline( testBoxProgram );

        // context_->createFramebuffers( piplineVertexBuffer_.get() );
    } catch ( const std::runtime_error& e ) {
        throw std::runtime_error( e.what() );
    }
}

void RenderVK::scene( const std::filesystem::path& path ) {
    scene_ = std::make_shared<vk::Scene>( path, context_,
                                          piplineVertexBuffer_.get() );
}

void RenderVK::setSwapInterval( int interval ) {

};

void RenderVK::keyPressEvent( unsigned int key ) {
    log::info( "{}", key );
    switch ( key ) {
        case 24: {  // == 'q'
            scene_->nextCamera();
            break;
        }
        case 87: {  // == 'w'
            scene_->camera().setMoveBit( FlycamMoveBits::FORWARD );
            break;
        }
        case 26: {  // == 'e'
            break;
        }
        case 27: {  // == 'r'
            break;
        }
        case 28: {  // == 't'
            break;
        }
        case 29: {  // == 'y'
            break;
        }
        case 30: {  // == 'u'
            break;
        }
        case 31: {  // == 'i'
            break;
        }
        case 32: {  // == 'o'
            break;
        }
        case 33: {  // == 'p'
            break;
        }
        case 34: {  // == '['
            break;
        }
        case 35: {  // == ']'
            break;
        }
        case 65: {  // == 'a'
            scene_->camera().setMoveBit( FlycamMoveBits::LEFT );
            break;
        }
        case 83: {  // == 's'
            scene_->camera().setMoveBit( FlycamMoveBits::BACKWARD );
            break;
        }
        case 68: {  // == 'd'
            scene_->camera().setMoveBit( FlycamMoveBits::RIGHT );
            break;
        }
        case 41: {  // == 'f'
            break;
        }
        case 42: {  // == 'g'
            // Window grab/release the mouse pointer
            holdMouse_ = !holdMouse_;
            if ( holdMouse_ ) {
                //XFixesHideCursor( display_, window_ );
            } else {
                //XFixesShowCursor( display_, window_ );
            }
            break;
        }
        case 43: {  // == 'h'
            break;
        }
        case 44: {  // == 'j'
            break;
        }
        case 45: {  // == 'k'
            break;
        }
        case 46: {  // == 'l'
            break;
        }
        case 47: {  // == ';'
            break;
        }
        case 48: {  // == '''
            break;
        }
        case 52: {  // == 'z'
            break;
        }
        default: {
            break;
        }
    }
}

void RenderVK::keyReleaseEvent( unsigned int key ) {
    switch ( key ) {
        case 87: {  // == 'w'
            scene_->camera().unsetMoveBit( FlycamMoveBits::FORWARD );
            break;
        }
        case 65: {  // == 'a'
            scene_->camera().unsetMoveBit( FlycamMoveBits::LEFT );
            break;
        }
        case 83: {  // == 's'
            scene_->camera().unsetMoveBit( FlycamMoveBits::BACKWARD );
            break;
        }
        case 68: {  // == 'd'
            scene_->camera().unsetMoveBit( FlycamMoveBits::RIGHT );
            break;
        }
        default: {
            break;
        }
    }
}

void RenderVK::mouseButtonPressEvent( unsigned int key ) {
}

void RenderVK::mouseButtonReleaseEvent( unsigned int key ) {
}

void RenderVK::mouseMoveEvent( unsigned int x, unsigned int y ) {
}

void RenderVK::mouseOffsetEvent( unsigned int x, unsigned int y ) {
    const int xOffset = holdMouseX_ - x;
    const int yOffset = holdMouseY_ - y;

#define MOUSE_SENSIVITY 0.002
    scene_->camera().rotate( xOffset * MOUSE_SENSIVITY,
                             yOffset * MOUSE_SENSIVITY );
}

auto RenderVK::drawTestCube( VkCommandBuffer cb ) -> void {
    // =================================
    // Get transformation matricies
    auto offset = algebra::translate( 0.0f, 0.0f, -2.0f );
    offset.transposeSelf();

    const auto [width, height] = context_->currentExtent();
    // NOTE: Choose right projection matrix!!!
    const auto proj = algebra::perspective<float>(
        50.0f, static_cast<float>( width ) / static_cast<float>( height ), 0.1f,
        100.0f );
    const auto viewMatrix = offset * proj;
    angle_ += timer_.floatFrameDuration() * 25.0f;
    algebra::vector3f ax{ 0.0f, 1.0f, 1.0f };
    ax.normalizeSelf();
    const auto modelMatrix = algebra::rotate( ax, angle_ );
    // =================================

    const VkViewport viewport{ .x = 0.0f,
                               .y = 0.0f,
                               .width = static_cast<float>( width ),
                               .height = static_cast<float>( height ),
                               .minDepth = 0.0f,
                               .maxDepth = 1.0f };
    vkCmdSetViewport( cb, 0, 1, &viewport );

    const VkRect2D scissor{ { .x = 0, .y = 0 },
                            { .width = width, .height = height } };
    vkCmdSetScissor( cb, 0, 1, &scissor );

    vkCmdBindPipeline( cb, VK_PIPELINE_BIND_POINT_GRAPHICS,
                       piplineTestBox_->pipeline() );

    vkCmdPushConstants( cb, piplineTestBox_->layout(),
                        VK_SHADER_STAGE_VERTEX_BIT, 0,
                        sizeof( algebra::matrix4f ), &viewMatrix );

    vkCmdPushConstants( cb, piplineTestBox_->layout(),
                        VK_SHADER_STAGE_VERTEX_BIT, sizeof( algebra::matrix4f ),
                        sizeof( algebra::matrix4f ), &modelMatrix );

    vkCmdDraw( cb, 36, 3, 0, 0 );
}

}  // namespace tire
