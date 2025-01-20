
#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <print>

#include <log/log.h>

#include "../device.h"

namespace tire::vk {

template <typename Derived>
struct RenderpassBase {
    friend Derived;

    RenderpassBase &operator=( const RenderpassBase &other ) = delete;
    RenderpassBase &operator=( RenderpassBase &&other ) = delete;

    ~RenderpassBase() {
        vkDestroyRenderPass( device_->handle(), renderPass_, nullptr );
    };

    [[nodiscard]] VkRenderPass handle() const { return renderPass_; }

    void init() {
        const auto self = static_cast<Derived *>( this );
        self->init();
    };

private:
    RenderpassBase( const vk::Device *device )
        : device_{ device } {}

    RenderpassBase( const RenderpassBase &other ) = delete;
    RenderpassBase( RenderpassBase &&other ) = delete;

protected:
    const vk::Device *device_;
    VkRenderPass renderPass_{ VK_NULL_HANDLE };
};

struct RenderpassSimple : RenderpassBase<RenderpassSimple> {
    using base_class = RenderpassBase<RenderpassSimple>;

    RenderpassSimple( const vk::Device *device )
        : base_class{ device } {
        init();
    }

    void init();
};

}  // namespace tire::vk
