
module;

#include <iostream>
#include <format>
#include <print>
#include <string>
#include <vector>
#include <algorithm>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "GLFW/glfw3.h"

export module screen:RenderVK;

import :Render;

namespace tire {

// static
VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
              void* pUserData) {
    std::print("validation layer: {}\n", pCallbackData->pMessage);
    return VK_FALSE;
}

VkResult vkCreateDebugUtilsMessenger(VkInstance instance,
                                     const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                     const VkAllocationCallbacks* pAllocator,
                                     VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

struct __vk_Render : Render {
        __vk_Render() {
            initExtensionProperties();
            initValidationLayers();
            createrInstance();
            initDebugMessenger();
            initPhysicalDevices();
        };

        ~__vk_Render() override {
            vkDestroyInstance(instance_, nullptr);
        };

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
        void initExtensionProperties() {
            uint32_t extCount;

            vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
            extensionProperties_.resize(extCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extCount, extensionProperties_.data());
        };

        void initValidationLayers() {
            uint32_t layerCount;

            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
            layerProperties_.resize(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, layerProperties_.data());
        }

        void createrInstance() {
            appInfo_.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo_.pApplicationName = "basic";
            appInfo_.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo_.pEngineName = "null engine";
            appInfo_.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo_.apiVersion = VK_API_VERSION_1_0;

            // extract extension names from array of VkExtensionProperties to array of strings
            std::vector<char*> extensions;
            std::for_each(extensionProperties_.begin(),
                          extensionProperties_.end(),
                          [&extensions](auto& ep) { extensions.push_back(ep.extensionName); });

            // extract validation layers names from  array of VkLayerProperties
            // to array of strings
            std::vector<char*> layers;
            std::for_each(layerProperties_.begin(), layerProperties_.end(), [&layers](auto& lp) {
                layers.push_back(lp.layerName);
            });

            instanceCreateInfo_.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            instanceCreateInfo_.pApplicationInfo = &appInfo_;
            instanceCreateInfo_.enabledLayerCount = layers.size();
            instanceCreateInfo_.ppEnabledLayerNames = layers.data();
            instanceCreateInfo_.enabledExtensionCount = extensions.size();
            instanceCreateInfo_.ppEnabledExtensionNames = extensions.data();

            auto res = vkCreateInstance(&instanceCreateInfo_, nullptr, &instance_);

            if (res != VK_SUCCESS) {
                std::print("can't create vk instance with code: {}\n", static_cast<int>(res));
                std::exit(0);
            } else {
                std::print("vk instance create success!!!\n");
            }
        }

        void initDebugMessenger() {
            VkDebugUtilsMessengerCreateInfoEXT dbgCreateInfo{};
            dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            dbgCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            dbgCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            dbgCreateInfo.pfnUserCallback = debugCallback;
            dbgCreateInfo.pUserData = nullptr;  // Optional

            if (vkCreateDebugUtilsMessenger(instance_, &dbgCreateInfo, nullptr, &debugMessenger_)
                != VK_SUCCESS) {
                std::print("failed to set up debug messenger!\n");
                std::exit(1);
            }
        }

        void initPhysicalDevices() {
            uint32_t devCount = 0;

            vkEnumeratePhysicalDevices(instance_, &devCount, nullptr);
            if (devCount == 0) {
                std::print("no vk physical devices in system\n");
                std::exit(1);
            } else {
                std::print("vk physical device count - {}\n", devCount);
            }

            physicalDevices_.resize(size_t(devCount));

            vkEnumeratePhysicalDevices(instance_, &devCount, physicalDevices_.data());

            for (auto& d : physicalDevices_) {
                VkPhysicalDeviceProperties devProps;
                vkGetPhysicalDeviceProperties(d, &devProps);
                physicalDevicesProperties_.push_back(devProps);
            }
        }
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
