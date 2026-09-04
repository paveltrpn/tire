#pragma once

#include <memory>

#include <vulkan/vulkan.h>
#include "vulkan/vulkan_core.h"
#include <vulkan/vk_enum_string_helper.h>

#include "vma/vk_mem_alloc.h"

#include "ui/ui.h"
#include "ui/billboard.h"
#include "ui/label.h"

import program;
import pipeline;
import vkresources;

namespace tire {

using namespace algebra;

#define VERTICIES_PER_QUAD 6
#define OUTPUT_QUADS_COUNT 128 * 2

// ============================================================================
// =================== QuadDrawBuffer =========================================
// ============================================================================

struct QuadDrawBuffer final {
    QuadDrawBuffer( size_t quadsCount );

    BufferObject _vBuf;
    BufferObject _tBuf;
    BufferObject _cBuf;

    uint32_t _primitievsCount{};
};

// ============================================================================
// =================== UiComponentVisitor =====================================
// ============================================================================

struct UiComponentVisitor final {
    UiComponentVisitor( VkCommandBuffer cb, QuadDrawBuffer& labelBuffer, QuadDrawBuffer& billboardBuffer );

    auto operator()( const tire::Label& item ) -> void;

    auto operator()( const tire::Billboard& item ) -> void;

    template <typename T>
    auto dispath( const T& item, QuadDrawBuffer& buffer ) -> void {
        const auto vOffset = buffer._primitievsCount * 3 * sizeof( float );
        const auto tOffset = buffer._primitievsCount * 2 * sizeof( float );
        const auto cOffset = buffer._primitievsCount * 4 * sizeof( float );

        const auto vDataPtr = reinterpret_cast<const void*>( item.verteciesData() );
        buffer._vBuf.memcpy( vDataPtr, item.bufferVerticesSize(), vOffset );

        const auto tDataPtr = reinterpret_cast<const void*>( item.texcrdsData() );
        buffer._tBuf.memcpy( tDataPtr, item.bufferTexcrdsSize(), tOffset );

        const auto cDataPtr = reinterpret_cast<const void*>( item.clrsData() );
        buffer._cBuf.memcpy( cDataPtr, item.bufferVertclrsSize(), cOffset );

        VkBufferCopy copyVrt{
            //
            .srcOffset = vOffset,
            .dstOffset = vOffset,
            .size = item.bufferVerticesSize(),
        };

        vkCmdCopyBuffer( _cb, buffer._vBuf.stagingBuffer(), buffer._vBuf.deviceBuffer(), 1, &copyVrt );

        VkBufferCopy copyTxc{
            //
            .srcOffset = tOffset,
            .dstOffset = tOffset,
            .size = item.bufferTexcrdsSize(),
        };

        vkCmdCopyBuffer( _cb, buffer._tBuf.stagingBuffer(), buffer._tBuf.deviceBuffer(), 1, &copyTxc );

        VkBufferCopy copyClrs{
            //
            .srcOffset = cOffset,
            .dstOffset = cOffset,
            .size = item.bufferVertclrsSize(),
        };

        vkCmdCopyBuffer( _cb, buffer._cBuf.stagingBuffer(), buffer._cBuf.deviceBuffer(), 1, &copyClrs );

        buffer._primitievsCount += item.lettersCount() * VERTICIES_PER_QUAD;
    }

    VkCommandBuffer _cb;

    QuadDrawBuffer& _labelBuffer;
    QuadDrawBuffer& _billboardBuffer;
};

// ============================================================================
// =================== UiVK ===================================================
// ============================================================================

struct UiVK final : tire::Ui {
    UiVK();

    auto upload( const VkCommandBuffer cb ) -> void;
    auto draw( const VkCommandBuffer cb ) -> void;
    auto flush() -> void override;

private:
    auto initDescriptorSets() -> void;
    auto initTextureSmpler() -> void;

private:
    std::shared_ptr<PipelineUi> _pipeline{};

    QuadDrawBuffer _labelBuffer{ OUTPUT_QUADS_COUNT };
    QuadDrawBuffer _billboardBuffer{ OUTPUT_QUADS_COUNT };

    std::shared_ptr<tire::TextureImage> _testImage;
    VkSampler _fontSampler{};
    VkDescriptorSet _fontDescSet{};
};

}  // namespace tire