
#include <stdexcept>
#include "vulkan/vulkan_core.h"
#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vk_enum_string_helper.h>

#include "context.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_CONTEXT_CPP{ true };

namespace tire::vk {}  // namespace tire::vk