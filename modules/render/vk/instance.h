
#pragma once

#include <vector>
#include <optional>
#include <vulkan/vulkan.h>
#include "vulkan/vulkan_core.h"

namespace tire::vk {

struct Instance final {
    Instance();

    Instance( const Instance &other ) = delete;
    Instance( Instance &&other ) = delete;
    Instance &operator=( const Instance &other ) = delete;
    Instance &operator=( Instance &&other ) = delete;

    ~Instance();

    [[nodiscard]] VkInstance handle() const;
    [[nodiscard]] std::pair<uint32_t, char const *const *> validationLayers()
        const;

    void info() const;

private:
    // Vulkan etity handles
    VkInstance instance_{ VK_NULL_HANDLE };
    VkDebugUtilsMessengerEXT debugMessenger_{ VK_NULL_HANDLE };

    std::vector<const char *> desiredValidationLayerList_{};

    std::vector<VkExtensionProperties> extensionProperties_{};
    std::vector<VkLayerProperties> layerProperties_{};
};

}  // namespace tire::vk
