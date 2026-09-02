
#include <format>
#include <memory>

#include "context.h"

#include "config/config.h"
#include "log/log.h"
#include "image/color.h"

namespace tire {

struct DepthImage;

void Context::init( uint32_t width, uint32_t height, Display* display, Window window ) {
    if ( _initSuccess ) {
        log::error()( "Warning: Singleton already initialized. Ignoring new arguments." );
    }

    std::call_once( _initFlag, [&]() -> void {
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

auto Context::instance() -> Context& {
    // memory_order_acquire ensures we see the fully constructed object
    Context* ptr = _instance.load();

    if ( !ptr ) {
        throw std::logic_error( "Singleton must be initialized via init() before calling getInstance()." );
    }
    return *ptr;
}

#ifdef SURFACE_X11
Context::Context( uint32_t width, uint32_t height, Display* display, Window window )
    : _width{ width }
    , _height{ height } {
    //
    _vkInstance = std::make_unique<VKInstance>( "VK_KHR_xlib_surface" );
    _vkDevice = std::make_unique<VKDevice>( _vkInstance.get() );
    _vkSurface = std::make_unique<VKSurfaceXLib>( _vkInstance.get(), _vkDevice.get(), width, height, display, window );
    _allocator = std::make_unique<VMAllocator>( _vkInstance.get(), _vkDevice.get() );
    _presentation = std::make_unique<Presentation>( _vkInstance.get(), _vkDevice.get() );
    _contextPools = std::make_unique<ContextPools>( _vkInstance.get(), _vkDevice.get() );

    makeSwapchain();
    initRenderPass();
    makeFrames();
    initCopyCommandBuffer();

    // Note that the order of clearValues should be identical to the order of your
    // attachments
    const auto colorString = tire::Config::instance().get<std::string>( "background_color" );
    const auto backgroundColor = Colorf( colorString );
    _clearValues[0].color = { { backgroundColor.r(), backgroundColor.g(), backgroundColor.b(), 1.0f } };
    _clearValues[1].depthStencil = { .depth = 1.0f, .stencil = 0 };
}
#elifdef SURFACE_WAYLAND
Context::Context( uint32_t width, uint32_t height, wl_display* display, wl_surface* surface )
    : width_{ width }
    , height_{ height } {
    //
    makeInstance( "VK_KHR_wayland_surface" );
    makeWaylandSurface( display, surface );
    initRest();
}
#endif

auto Context::releaseContext() -> void {
    log::info()( "Release vulkan context..." );

    vkDestroyFence( device(), _copyCommandFence, nullptr );

    for ( auto i = 0; i < _framesCount; i++ ) {
        vkDestroySemaphore( device(), _frames[i]._imageAvailableSemaphore, nullptr );
        vkDestroySemaphore( device(), _frames[i]._renderFinishedSemaphore, nullptr );
        vkDestroyFence( device(), _frames[i]._inFlightFence, nullptr );
        vkDestroyFramebuffer( device(), _frames[i]._framebuffer, nullptr );
        vkDestroyImageView( device(), _frames[i]._view, nullptr );
    }

    vkDestroySwapchainKHR( device(), _swapchain, nullptr );

    _presentation.reset();
    _allocator.reset();
    _vkSurface.reset();
    _vkDevice.reset();
    _vkInstance.reset();
};

}  // namespace tire
