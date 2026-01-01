
module;

#include <format>

#define SURFACE_X11

#ifdef SURFACE_X11
#define VK_USE_PLATFORM_XLIB_KHR
#elifdef SURFACE_WAYLAND
#define VK_USE_PLATFORM_WAYLAND_KHR
#endif

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

export module context:surface;

import :context;

import log;

namespace tire {

#ifdef SURFACE_X11

auto Context::makeXlibSurface( Display *display, Window window ) -> void {
    VkXlibSurfaceCreateInfoKHR xlibSurfInfo{};
    xlibSurfInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    xlibSurfInfo.dpy = display;
    xlibSurfInfo.window = window;

    if ( const auto err = vkCreateXlibSurfaceKHR( instance_, &xlibSurfInfo, nullptr, &surface_ ); err != VK_SUCCESS ) {
        throw std::runtime_error(
          std::format( "failed to create xlib surface with code {}\n!", string_VkResult( err ) ) );
    } else {
        log::info( "Surface === xlib surface created!" );
    }
}

#elifdef SURFACE_WAYLAND

auto Context::makeWaylandSurface( wl_display *display, wl_surface *surface ) -> void {
    VkWaylandSurfaceCreateInfoKHR wlSurfInfo{};
    wlSurfInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    wlSurfInfo.display = display;
    wlSurfInfo.surface = surface;

    if ( const auto err = vkCreateWaylandSurfaceKHR( instance_, &wlSurfInfo, nullptr, &surface_ ); err != VK_SUCCESS ) {
        throw std::runtime_error(
          std::format( "failed to create wayland surface with code {}\n!", string_VkResult( err ) ) );
    } else {
        log::info( "Surface === xlib surface created!" );
    }
}

#endif

}  // namespace tire
