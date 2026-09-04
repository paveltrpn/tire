
#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "event/event.h"
#include "event/eventobserver.h"

#include "timer.h"
#include "ui/ui.h"
#include "test_box/test_box.h"

import graph;

namespace tire {

struct RenderVK final : public EventObserver {
    explicit RenderVK();

    RenderVK( const RenderVK& other ) = delete;
    RenderVK( RenderVK&& other ) = delete;

    auto operator=( const RenderVK& other ) -> RenderVK& = delete;
    auto operator=( RenderVK&& other ) -> RenderVK& = delete;

    ~RenderVK() = default;

    [[nodiscard]]
    auto isRun() -> bool;

    auto preLoop() -> void;
    auto frame() -> void;
    auto postLoop() -> void;

    auto handleEvent( std::shared_ptr<EventBase> event ) -> void override;

    auto attachScene( std::shared_ptr<Scene> scene ) -> void;

private:
    bool _run{};

    uint32_t _currentFrame{ 0 };

    // Scene.
    std::shared_ptr<Scene> _scene{};

    // Ui.
    std::shared_ptr<UiVK> _ui{};

    // Test box.
    std::shared_ptr<TestBox> _testBox{};

    Timer _timer{};

    bool _holdMouse{ false };
};

}  // namespace tire
