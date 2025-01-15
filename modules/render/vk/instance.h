
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

    [[nodiscard]] VkInstance instance() const;
    [[nodiscard]] std::pair<uint32_t, char *const *const> validationLayersInfo()
        const;

    void info() const;

private:
    // pass std::nullopt to enable all available exensions
    std::vector<char *> makeExtensionsList(
        std::optional<std::vector<std::string>> list );
    // pass std::nullopt to enable all available validation layers.
    std::vector<char *> makeValidationLayersList(
        std::optional<std::vector<std::string>> list );

private:
    VkInstance instance_{ VK_NULL_HANDLE };
    VkDebugUtilsMessengerEXT debugMessenger_{ VK_NULL_HANDLE };

    std::vector<char *> validationLayersNames_{};
    std::vector<char *> extensionsNames_{};

    std::vector<VkExtensionProperties> extensionProperties_{};
    std::vector<VkLayerProperties> layerProperties_{};
    VkApplicationInfo appInfo_{};
    VkDebugUtilsMessengerCreateInfoEXT dbgCreateInfo_{};
    VkInstanceCreateInfo instanceCreateInfo_{};
};

}  // namespace tire::vk
