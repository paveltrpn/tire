
module;

#include <iostream>
#include <format>
#include <print>
#include <string>
#include <vector>
#include <algorithm>
#include <expected>
#include <variant>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "GLFW/glfw3.h"

export module render:RenderVK;

import config;

import :Render;

namespace tire {

// static
VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
              void* pUserData) {
    std::print("validation layer debug:\t\t{}\n", pCallbackData->pMessage);
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
        __vk_Render(const tire::Config& config) : Render{ config } {
            applicationName_ = config_.getString("application_name");
            engineName_ = config_.getString("engine_name");

            appInfo_.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo_.pApplicationName = applicationName_.data();
            appInfo_.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo_.pEngineName = engineName_.data();
            appInfo_.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo_.apiVersion = VK_API_VERSION_1_0;

            dbgCreateInfo_.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            dbgCreateInfo_.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                             | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                             | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            dbgCreateInfo_.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                         | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                         | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            dbgCreateInfo_.pfnUserCallback = debugCallback;
            dbgCreateInfo_.pUserData = nullptr;  // Optional

            createInstance();
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

        // pass std::nullopt to enable all avaible exensions
        std::vector<char*> makeExtensionsList(std::optional<std::vector<std::string>> list) {
            std::vector<char*> rt{};

            enumerateExtensionProperties();

            if (list) {
                for (const auto& name : list.value()) {
                    auto res
                      = std::find_if(extensionProperties_.begin(),
                                     extensionProperties_.end(),
                                     [name](auto& ep) -> bool { return ep.extensionName == name; });
                    if (res != extensionProperties_.end()) {
                        rt.push_back((*res).extensionName);
                    } else {
                        std::print("extension \"{}\" not supported\n", name);
                    }
                }
            } else {
                for (auto& ep : extensionProperties_) {
                    rt.push_back(ep.extensionName);
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

        // pass std::nullopt to enable all avaible validation layers.
        // may cause instance creation error, for example:
        // "Requested layer "VK_LAYER_VALVE_steam_overlay_32" was wrong bit-type!"
        std::vector<char*> makeValidationLayersList(std::optional<std::vector<std::string>> list) {
            std::vector<char*> rt{};

            enumerateValidationLayers();

            if (list) {
                for (const auto& name : list.value()) {
                    auto res = std::find_if(
                      layerProperties_.begin(), layerProperties_.end(), [name](auto& lp) -> bool {
                          return lp.layerName == name;
                      });
                    if (res != layerProperties_.end()) {
                        rt.push_back((*res).layerName);
                    } else {
                        std::print("validation layer \"{}\" not supported\n", name);
                    }
                }
            } else {
                for (auto& lp : layerProperties_) {
                    rt.push_back(lp.layerName);
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

        void createInstance(bool enableValidationLayers = true) {
            instanceCreateInfo_.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            instanceCreateInfo_.pApplicationInfo = &appInfo_;

            //
            // validation layers
            //
            std::vector<std::string> vllist{ "VK_LAYER_INTEL_nullhw",
                                             "VK_LAYER_MESA_device_select",
                                             "VK_LAYER_MESA_overlay",
                                             "VK_LAYER_NV_optimus",
                                             //"VK_LAYER_VALVE_steam_fossilize_32",
                                             "VK_LAYER_VALVE_steam_fossilize_64",
                                             //"VK_LAYER_VALVE_steam_overlay_32",
                                             "VK_LAYER_VALVE_steam_overlay_64" };

            auto layers = makeValidationLayersList(vllist);

            if (enableValidationLayers) {
                instanceCreateInfo_.enabledLayerCount = static_cast<uint32_t>(layers.size());
                instanceCreateInfo_.ppEnabledLayerNames = layers.data();
                instanceCreateInfo_.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&dbgCreateInfo_;
            }

            //
            // extensions
            //
            std::vector<std::string> eplist{
                "VK_KHR_surface", "VK_KHR_xlib_surface", "VK_EXT_debug_report", "VK_EXT_debug_utils"
            };

            auto extensions = makeExtensionsList(std::nullopt);

            instanceCreateInfo_.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
            instanceCreateInfo_.ppEnabledExtensionNames = extensions.data();

            //
            // instance creation
            //
            auto res = vkCreateInstance(&instanceCreateInfo_, nullptr, &instance_);

            if (res != VK_SUCCESS) {
                throw std::runtime_error(
                  std::format("can't create vk instance with code: {}\n", static_cast<int>(res)));
            }

            if (enableValidationLayers) {
                if (vkCreateDebugUtilsMessenger(
                      instance_, &dbgCreateInfo_, nullptr, &debugMessenger_)
                    != VK_SUCCESS) {
                    throw std::runtime_error("failed to set up debug messenger!\n");
                }
            }
        }

        void enumeratePhysicalDevices() {
            uint32_t devCount = 0;

            vkEnumeratePhysicalDevices(instance_, &devCount, nullptr);
            if (devCount == 0) {
                throw std::runtime_error("no vk physical devices in system\n");
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

        std::string applicationName_;
        std::string engineName_;

        // all vk structures must be zero initialized
        VkInstance instance_{};
        VkApplicationInfo appInfo_{};
        VkInstanceCreateInfo instanceCreateInfo_{};
        VkDebugUtilsMessengerEXT debugMessenger_{};
        VkDebugUtilsMessengerCreateInfoEXT dbgCreateInfo_{};

        std::vector<VkExtensionProperties> extensionProperties_;
        std::vector<VkLayerProperties> layerProperties_;
        std::vector<VkPhysicalDevice> physicalDevices_;
        std::vector<VkPhysicalDeviceProperties> physicalDevicesProperties_;
};

}  // namespace tire
