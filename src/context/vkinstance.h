
#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace tire {

struct VKInstance final {
    VKInstance();

    VKInstance( const VKInstance &other ) = delete;
    VKInstance( VKInstance &&other ) = delete;

    auto operator=( const VKInstance &other ) -> VKInstance & = delete;
    auto operator=( VKInstance &&other ) -> VKInstance & = delete;

    ~VKInstance() = default;

    [[nodiscard]] auto get() -> VkInstance;

private:
    VkInstance vkInstance_{ VK_NULL_HANDLE };
    VkDebugUtilsMessengerEXT debugMessenger_{ VK_NULL_HANDLE };
    std::vector<const char *> desiredValidationLayerList_{};
    std::vector<VkExtensionProperties> extensionProperties_{};
    std::vector<VkLayerProperties> layerProperties_{};
};

}
