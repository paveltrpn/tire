
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
    vkDevice_ = std::make_unique<VKDevice>( vkInstance_.get() );
    vkSurface_ = std::make_unique<VKSurfaceXLib>( vkInstance_.get(), vkDevice_.get(), width, height, display, window );

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
    vkDestroyFence( device(), copyCommandFence_, nullptr );
    vkDestroyDescriptorPool( device(), descriptorPool_, nullptr );

    for ( auto i = 0; i < framesCount_; i++ ) {
        vkDestroySemaphore( device(), frames_[i].imageAvailableSemaphore_, nullptr );
        vkDestroySemaphore( device(), frames_[i].renderFinishedSemaphore_, nullptr );
        vkDestroyFence( device(), frames_[i].inFlightFence_, nullptr );
        vkDestroyFramebuffer( device(), frames_[i].framebuffer_, nullptr );
        vkDestroyImageView( device(), frames_[i].view_, nullptr );
    }

    vkDestroyCommandPool( device(), commandPool_, nullptr );

    vmaDestroyAllocator( allocator_ );

    vkDestroySwapchainKHR( device(), swapchain_, nullptr );
    vkDestroyDevice( device(), nullptr );
}

}  // namespace tire
