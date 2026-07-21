
#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "event/eventobserver.h"

#include "timer.h"
#include "test_box/test_box.h"
#include "ui/ui.h"

// using namespace std::chrono_literals;

namespace tire {

struct RenderVK final : public EventObserver {
    explicit RenderVK();

    RenderVK( const RenderVK &other ) = delete;
    RenderVK( RenderVK &&other ) = delete;

    auto operator=( const RenderVK &other ) -> RenderVK & = delete;
    auto operator=( RenderVK &&other ) -> RenderVK & = delete;

    ~RenderVK() = default;

    [[nodiscard]] auto isRun() -> bool;

    auto displayRenderInfo() -> void;

    auto preLoop() -> void;
    auto frame() -> void;
    auto postLoop() -> void;

    auto handleEvent( Event *event ) -> void override;

public:
    auto holdMouse() -> bool;
    auto keyPressEvent( unsigned int key ) -> void;
    auto keyReleaseEvent( unsigned int key ) -> void;
    auto mouseButtonPressEvent( unsigned int key ) -> void;
    auto mouseButtonReleaseEvent( unsigned int key ) -> void;

    // Call when mouse moves free upon window. "x" and "y"
    // represent current cursor position in window coordinates
    auto mouseMoveEvent( double x, double y ) -> void;

    // Call when mouse holds in defined position. "x" and "y"
    // represent current cursor ofssets.
    auto mouseOffsetEvent( double x, double y, double holdX, double holdY ) -> void;

private:
    bool run_{};

    uint32_t currentFrame_{ 0 };

    // Ui.
    std::shared_ptr<UiVK> ui_{};

    // Test box.
    std::shared_ptr<TestBox> testBox_{};

    Timer timer_{};

    bool holdMouse_{ false };
};

}  // namespace tire
