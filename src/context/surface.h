
#pragma once

#include <vector>
#include <optional>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <vulkan/vulkan.h>

namespace tire {

struct VKInstance;

struct VKSurface {
    VKSurface( const VKInstance *instance );

    VKSurface( const VKSurface &other ) = delete;
    VKSurface( VKSurface &&other ) = delete;

    auto operator=( const VKSurface &other ) -> VKSurface & = delete;
    auto operator=( VKSurface &&other ) -> VKSurface & = delete;

    ~VKSurface() = default;

    [[nodiscard]] auto get() const -> VkSurfaceKHR;
    [[nodiscard]] auto surfaceFormat() const -> const VkSurfaceFormatKHR &;

protected:
    const VKInstance *instance_{};

    VkSurfaceKHR surface_{ VK_NULL_HANDLE };
    VkSurfaceFormatKHR surfaceFormat_{};
};

struct VKSurfaceXLib final : public VKSurface {
    VKSurfaceXLib( const VKInstance *instance, uint32_t width, uint32_t height, Display *display, Window window );
};

/*
#include <wayland-client.h>

struct VKSurfaceWayland final {
    VKSurfaceWayland( wl_display *display, wl_surface *surface );

    VKSurfaceWayland( const VKSurfaceWayland &other ) = delete;
    VKSurfaceWayland( VKSurfaceWayland &&other ) = delete;

    auto operator=( const VKSurfaceWayland &other ) -> VKSurfaceWayland & = delete;
    auto operator=( VKSurfaceWayland &&other ) -> VKSurfaceWayland & = delete;

    ~VKSurfaceWayland() = default;
};
*/

}  // namespace tire