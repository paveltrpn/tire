
#include "present.h"

namespace tire::vk {

Present::Present( const vk::Device *device, const vk::Swapchain *swapchain )
    : device_{ device }
    , swapchain_{ swapchain } {
}

}  // namespace tire::vk
