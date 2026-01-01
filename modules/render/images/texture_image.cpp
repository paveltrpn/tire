
module;

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "context/context.h"

export module render:texture_image;

namespace tire::vk {

struct TextureImage final {
private:
    const Context *context_{};
};

}  // namespace tire::vk
