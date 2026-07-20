
#include <exception>

#define SURFACE_X11

#ifdef SURFACE_X11
#define VK_USE_PLATFORM_XLIB_KHR
#elifdef SURFACE_WAYLAND
#define VK_USE_PLATFORM_WAYLAND_KHR
#endif

#include <vulkan/vk_enum_string_helper.h>

#include "vkinstance.h"
#include "surface.h"
#include "log/log.h"

namespace tire {

auto VKSurface::get() const -> VkSurfaceKHR {
    return surface_;
}

auto VKSurface::surfaceFormat() const -> const VkSurfaceFormatKHR & {
    return surfaceFormat_;
}

VKSurfaceXLib::VKSurfaceXLib( const VKInstance *instance, uint32_t width, uint32_t height, Display *display,
                              Window window )
    : VKSurface{ instance } {
    VkXlibSurfaceCreateInfoKHR xlibSurfInfo{};
    xlibSurfInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    xlibSurfInfo.dpy = display;
    xlibSurfInfo.window = window;

    if ( const auto err = vkCreateXlibSurfaceKHR( instance_->get(), &xlibSurfInfo, nullptr, &surface_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create xlib surface with code {}\n!", string_VkResult( err ) ) );
    } else {
        log::info()( "xlib surface created!" );
    }
}

/*
VKSurfaceWayland::VKSurfaceWayland( wl_display *display, wl_surface *surface ) {
    VkWaylandSurfaceCreateInfoKHR wlSurfInfo{};
    wlSurfInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    wlSurfInfo.display = display;
    wlSurfInfo.surface = surface;

    if ( const auto err = vkCreateWaylandSurfaceKHR( instance_, &wlSurfInfo, nullptr, &surface_ ); err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create wayland surface with code {}\n!", string_VkResult( err ) ) );
    } else {
        log::info()( "wayland surface created!" );
    }
}
*/

}  // namespace tire
