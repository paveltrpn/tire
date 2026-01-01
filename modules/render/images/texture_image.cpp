
module;

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

export module render:texture_image;

import context;

namespace tire::vk {

struct TextureImage final {
private:
    const Context *context_{};
};

}  // namespace tire::vk
