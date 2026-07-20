
#include <format>
#include <memory>

#include "context.h"

#include "config/config.h"
#include "log/log.h"
#include "image/image.h"

#include "command_routine.h"

namespace tire {

struct DepthImage;

void Context::init( uint32_t width, uint32_t height, Display *display, Window window ) {
    if ( _initSuccess ) {
        log::error()( "Warning: Singleton already initialized. Ignoring new arguments." );
    }

    std::call_once( _initFlag, [&]() {
        // We deliberately use 'new' and do not delete.
        // This is intentional. It solves the Static Destruction Order Fiasco.
        // If your Singleton is destroyed during program shutdown, and another static
        // object's destructor tries to use it, your program crashes. By leaking
        // the pointer, the Singleton survives past the end of the program, and the OS automatically
        // reclaims the memory when the process exits anyway.
        _instance.store( new Context( width, height, display, window ) );
        _initSuccess = true;
    } );
}

Context &Context::instance() {
    // memory_order_acquire ensures we see the fully constructed object
    Context *ptr = _instance.load();

    if ( !ptr ) {
        throw std::logic_error( "Singleton must be initialized via init() before calling getInstance()." );
    }
    return *ptr;
}

#ifdef SURFACE_X11
Context::Context( uint32_t width, uint32_t height, Display *display, Window window )
    : width_{ width }
    , height_{ height } {
    //
    vkInstance_ = std::make_unique<VKInstance>( "VK_KHR_xlib_surface" );

    makeXlibSurface( display, window );
    initRest();
}
#elifdef SURFACE_WAYLAND
Context::Context( uint32_t width, uint32_t height, wl_display *display, wl_surface *surface )
    : width_{ width }
    , height_{ height } {
    //
    makeInstance( "VK_KHR_wayland_surface" );
    makeWaylandSurface( display, surface );
    initRest();
}
#endif

// Destroy all Vulkan context here.
Context::~Context() {
    vkDestroyFence( device_, copyCommandFence_, nullptr );
    vkDestroyDescriptorPool( device_, descriptorPool_, nullptr );

    for ( auto i = 0; i < framesCount_; i++ ) {
        vkDestroySemaphore( device_, frames_[i].imageAvailableSemaphore_, nullptr );
        vkDestroySemaphore( device_, frames_[i].renderFinishedSemaphore_, nullptr );
        vkDestroyFence( device_, frames_[i].inFlightFence_, nullptr );
        vkDestroyFramebuffer( device_, frames_[i].framebuffer_, nullptr );
        vkDestroyImageView( device_, frames_[i].view_, nullptr );
    }

    vkDestroyCommandPool( device_, commandPool_, nullptr );
    vkDestroySurfaceKHR( vkInstance_->get(), surface_, nullptr );

    vmaDestroyAllocator( allocator_ );

    vkDestroySwapchainKHR( device_, swapchain_, nullptr );
    vkDestroyDevice( device_, nullptr );
}

#ifdef SURFACE_X11
auto Context::makeXlibSurface( Display *display, Window window ) -> void {
    VkXlibSurfaceCreateInfoKHR xlibSurfInfo{};
    xlibSurfInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    xlibSurfInfo.dpy = display;
    xlibSurfInfo.window = window;

    if ( const auto err = vkCreateXlibSurfaceKHR( vkInstance_->get(), &xlibSurfInfo, nullptr, &surface_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create xlib surface with code {}\n!", string_VkResult( err ) ) );
    } else {
        log::info()( "xlib surface created!" );
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
        log::info()( "wayland surface created!" );
    }
}
#endif

}  // namespace tire
