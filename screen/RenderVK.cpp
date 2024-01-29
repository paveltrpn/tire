
module;

#include <iostream>
#include <format>
#include <print>
#include <string>
#include <vector>
#include <algorithm>
#include <expected>

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
            appInfo_.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo_.pApplicationName = "basic";
            appInfo_.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo_.pEngineName = "null engine";
            appInfo_.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo_.apiVersion = VK_API_VERSION_1_0;

            enumerateExtensionProperties();
            enumerateValidationLayers();
            createrInstance();
            initDebugMessenger();
            enumeratePhysicalDevices();
        };

        ~__vk_Render() override {
            vkDestroyInstance(instance_, nullptr);
        };

        void displayRenderInfo() override {
            displayExtensionProperties();
            displayValidationLayerProperties();
            displayPhysicalDeviceProperties();
        }

    protected:
        void enumerateExtensionProperties() {
            uint32_t extCount;

            vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
            extensionProperties_.resize(extCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extCount, extensionProperties_.data());
        };

        std::expected<std::vector<char*>, std::string> makeExtensionsList(
          const std::vector<std::string>& list) {
            std::vector<char*> rt{};

            for (const auto& name : list) {
                auto res
                  = std::find_if(extensionProperties_.begin(),
                                 extensionProperties_.end(),
                                 [name](auto& ep) -> bool { return ep.extensionName == name; });
                if (res == extensionProperties_.end()) {
                    return std::unexpected(name);
                } else {
                    rt.push_back((*res).extensionName);
                }
            }

            return rt;
        }

        void displayExtensionProperties() {
            std::print("Instance extensions count: {}\n"
                       "==========================\n",
                       extensionProperties_.size());
            for (auto& prop : extensionProperties_) {
                std::print("\t{} | revision: {}\n", prop.extensionName, prop.specVersion);
            }
        }

        void enumerateValidationLayers() {
            uint32_t layerCount;

            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
            layerProperties_.resize(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, layerProperties_.data());
        }

        std::expected<std::vector<char*>, std::string> makeValidationLayersList(
          const std::vector<std::string>& list) {
            std::vector<char*> rt;

            for (const auto& name : list) {
                auto res = std::find_if(layerProperties_.begin(),
                                        layerProperties_.end(),
                                        [name](auto& lp) -> bool { return lp.layerName == name; });
                if (res == layerProperties_.end()) {
                    return std::unexpected(name);
                } else {
                    rt.push_back((*res).layerName);
                }
            }

            return rt;
        }

        void displayValidationLayerProperties() {
            std::print("Layers count: {}\n"
                       "=============\n",
                       layerProperties_.size());
            for (auto& layer : layerProperties_) {
                std::print("\t{} | specVersion: {}\n\t{}\n",
                           layer.layerName,
                           layer.specVersion,
                           layer.description);
            }
        }

        void createrInstance() {
            instanceCreateInfo_.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            instanceCreateInfo_.pApplicationInfo = &appInfo_;

            auto layers = makeValidationLayersList({ "VK_LAYER_INTEL_nullhw",
                                                     "VK_LAYER_MESA_device_select",
                                                     "VK_LAYER_MESA_overlay",
                                                     "VK_LAYER_NV_optimus",
                                                     //"VK_LAYER_VALVE_steam_fossilize_32",
                                                     "VK_LAYER_VALVE_steam_fossilize_64",
                                                     //"VK_LAYER_VALVE_steam_overlay_32",
                                                     "VK_LAYER_VALVE_steam_overlay_64" });
            if (!layers.has_value()) {
                std::print(
                  "no such vulkan validation layer: \"{}\"! contunue instance creation with zero "
                  "enabled validation layers!\n",
                  layers.error());

                instanceCreateInfo_.enabledLayerCount = 0;
                instanceCreateInfo_.ppEnabledLayerNames = nullptr;
            } else {
                instanceCreateInfo_.enabledLayerCount = (*layers).size();
                instanceCreateInfo_.ppEnabledLayerNames = (*layers).data();
            }

            auto extensions = makeExtensionsList({ "VK_KHR_surface",
                                                   "VK_KHR_xlib_surface",
                                                   "VK_EXT_debug_report",
                                                   "VK_EXT_debug_utils" });
            if (!extensions.has_value()) {
                std::print("no such vulkan extension: \"{}\"! contunue instance creation with zero "
                           "enabled extensions!\n",
                           extensions.error());

                instanceCreateInfo_.enabledExtensionCount = 0;
                instanceCreateInfo_.ppEnabledExtensionNames = nullptr;
            } else {
                instanceCreateInfo_.enabledExtensionCount = (*extensions).size();
                instanceCreateInfo_.ppEnabledExtensionNames = (*extensions).data();
            }

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

        void enumeratePhysicalDevices() {
            uint32_t devCount = 0;

            vkEnumeratePhysicalDevices(instance_, &devCount, nullptr);
            if (devCount == 0) {
                std::print("no vk physical devices in system\n");
                std::exit(1);
            }

            physicalDevices_.resize(size_t(devCount));

            vkEnumeratePhysicalDevices(instance_, &devCount, physicalDevices_.data());

            for (auto& d : physicalDevices_) {
                VkPhysicalDeviceProperties devProps;
                vkGetPhysicalDeviceProperties(d, &devProps);
                physicalDevicesProperties_.push_back(devProps);
            }
        }

        void displayPhysicalDeviceProperties() {
            std::print("physical devices:\n"
                       "================\n");
            for (auto& d : physicalDevicesProperties_) {
                std::print("\tname: {}\n\tid: {}\n\tvendor id: {}\n\tapi: {}\n",
                           d.deviceName,
                           d.deviceID,
                           d.vendorID,
                           d.apiVersion);
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
