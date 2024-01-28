
module;

#include <iostream>
#include <format>
#include <print>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "GLFW/glfw3.h"

export module screen:RenderVK;

import :Render;

namespace tire {

struct __vk_Render : Render {
        __vk_Render() = default;

        __vk_Render(const __vk_Render& rhs) = delete;
        __vk_Render(__vk_Render&& ths) = delete;

        __vk_Render& operator=(const __vk_Render& rhs) = delete;
        __vk_Render& operator=(__vk_Render&& rhs) = delete;

        ~__vk_Render() override = default;

        void displayRenderInfo() override {
            std::print("Instance extensions count: {}\n", extensionProperties_.size());
            std::print("==========================\n");
            for (auto& prop : extensionProperties_) {
                std::print("\t{} | revision: {}\n", prop.extensionName, prop.specVersion);
            }

            std::print("Layers count: {}\n", layerProperties_.size());
            std::print("============\n");
            for (auto& layer : layerProperties_) {
                std::print("\t{} | specVersion: {}\n\t{}\n",
                           layer.layerName,
                           layer.specVersion,
                           layer.description);
            }
            /*
            std::print("physical devices names:\n");
            for (auto& d : physicalDevicesProperties_) {
                std::print("\t{}\n", d.deviceName);
            }
            */
        }

    protected:
        // all vk structures must be zero initialized
        VkInstance instance_{};
        VkApplicationInfo appInfo_{};
        VkInstanceCreateInfo instanceCreateInfo_{};
        VkDebugUtilsMessengerEXT debugMessenger_;

        std::vector<VkExtensionProperties> extensionProperties_;
        std::vector<VkLayerProperties> layerProperties_;
        std::vector<VkPhysicalDevice> physicalDevices_;
        std::vector<VkPhysicalDeviceProperties> physicalDevicesProperties_;
};

}  // namespace tire
