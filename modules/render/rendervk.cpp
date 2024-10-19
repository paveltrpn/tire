
module;

#include <print>
#include <string>
#include <vector>
#include <algorithm>
#include <expected>
#include <variant>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

export module render:rendervk;

import :render;
import config;

namespace tire {

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
              void* pUserData) {
    std::print("validation layer debug:\t\t{}\n", pCallbackData->pMessage);
    return VK_FALSE;
}

static VkResult vkCreateDebugUtilsMessenger(VkInstance instance,
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

export {
    struct RenderVK final : Render {
            RenderVK();
            ~RenderVK() override;

            void displayRenderInfo() override {
                displayExtensionProperties();
                displayValidationLayerProperties();

                for (size_t i = 0; i < physicalDevices_.size(); ++i) {
                    displayPhysicalDeviceProperties(i);
                    displayPhysicalDeviceFeatures(i);
                    displayPhysicalDeviceFamiliesProperties(i);
                }
            }

            void initMainLoop() override {};
            void preFrame() override {};
            void frame() override {};
            void postFrame() override {};
            void swapBuffers() override {};

            void setSwapInterval(int interval) override {};

            void appendToRenderList(std::shared_ptr<tire::Node<point_scalar_type>> node) override;

        private:
            void enumerateExtensionProperties() {
                uint32_t extCount;

                vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
                extensionProperties_.resize(extCount);
                vkEnumerateInstanceExtensionProperties(
                  nullptr, &extCount, extensionProperties_.data());
            };

            // pass std::nullopt to enable all avaible exensions
            std::vector<char*> makeExtensionsList(std::optional<std::vector<std::string>> list) {
                std::vector<char*> rt{};

                enumerateExtensionProperties();

                if (list) {
                    for (const auto& name : list.value()) {
                        auto res = std::find_if(
                          extensionProperties_.begin(),
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

            void displayExtensionProperties();

            void enumerateValidationLayers() {
                uint32_t layerCount;

                vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
                layerProperties_.resize(layerCount);
                vkEnumerateInstanceLayerProperties(&layerCount, layerProperties_.data());
            }

            // pass std::nullopt to enable all avaible validation layers.
            // may cause instance creation error, for example:
            // "Requested layer "VK_LAYER_VALVE_steam_overlay_32" was wrong bit-type!"
            std::vector<char*> makeValidationLayersList(
              std::optional<std::vector<std::string>> list) {
                std::vector<char*> rt{};

                enumerateValidationLayers();

                if (list) {
                    for (const auto& name : list.value()) {
                        auto res
                          = std::find_if(layerProperties_.begin(),
                                         layerProperties_.end(),
                                         [name](auto& lp) -> bool { return lp.layerName == name; });
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

            void displayValidationLayerProperties();

            void createInstance() {
                // VkApplicationInfo
                appInfo_.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
                appInfo_.pApplicationName = applicationName_.data();
                appInfo_.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
                appInfo_.pEngineName = engineName_.data();
                appInfo_.engineVersion = VK_MAKE_VERSION(1, 0, 0);
                appInfo_.apiVersion = VK_API_VERSION_1_0;

                // VkDebugCreateInfo
                dbgCreateInfo_.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
                dbgCreateInfo_.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                                 | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                                 | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
                dbgCreateInfo_.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                             | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                             | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
                dbgCreateInfo_.pfnUserCallback = debugCallback;
                dbgCreateInfo_.pUserData = nullptr;  // Optional

                // VkInstanceCreateInfo
                instanceCreateInfo_.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
                instanceCreateInfo_.pApplicationInfo = &appInfo_;

                // validation layers
                std::vector<std::string> vllist{ "VK_LAYER_INTEL_nullhw",
                                                 "VK_LAYER_MESA_device_select",
                                                 "VK_LAYER_MESA_overlay",
                                                 "VK_LAYER_NV_optimus",
                                                 //"VK_LAYER_VALVE_steam_fossilize_32",
                                                 "VK_LAYER_VALVE_steam_fossilize_64",
                                                 //"VK_LAYER_VALVE_steam_overlay_32",
                                                 "VK_LAYER_VALVE_steam_overlay_64" };

                validationLayersNames_ = makeValidationLayersList(vllist);

                if (enableValidationLayers_) {
                    instanceCreateInfo_.enabledLayerCount
                      = static_cast<uint32_t>(validationLayersNames_.size());
                    instanceCreateInfo_.ppEnabledLayerNames = validationLayersNames_.data();
                    instanceCreateInfo_.pNext
                      = (VkDebugUtilsMessengerCreateInfoEXT*)&dbgCreateInfo_;
                }

                // extensions
                std::vector<std::string> eplist{ "VK_KHR_surface",
                                                 "VK_KHR_xlib_surface",
                                                 "VK_EXT_debug_report",
                                                 "VK_EXT_debug_utils" };

                extensionsNames_ = makeExtensionsList(std::nullopt);

                instanceCreateInfo_.enabledExtensionCount
                  = static_cast<uint32_t>(extensionsNames_.size());
                instanceCreateInfo_.ppEnabledExtensionNames = extensionsNames_.data();

                // instance creation
                auto res = vkCreateInstance(&instanceCreateInfo_, nullptr, &instance_);

                if (res != VK_SUCCESS) {
                    throw std::runtime_error(std::format("can't create vk instance with code: {}\n",
                                                         static_cast<int>(res)));
                }

                if (enableValidationLayers_) {
                    if (vkCreateDebugUtilsMessenger(
                          instance_, &dbgCreateInfo_, nullptr, &debugMessenger_)
                        != VK_SUCCESS) {
                        throw std::runtime_error("failed to set up debug messenger!\n");
                    }
                }
            }

            void initPhysicalDevices() {
                uint32_t devCount{};

                vkEnumeratePhysicalDevices(instance_, &devCount, nullptr);
                if (devCount == 0) {
                    throw std::runtime_error("no vk physical devices in system\n");
                }

                physicalDevices_.resize(size_t(devCount));

                vkEnumeratePhysicalDevices(instance_, &devCount, physicalDevices_.data());

                for (auto& device : physicalDevices_) {
                    VkPhysicalDeviceProperties devProps;
                    VkPhysicalDeviceFeatures devFeatures;

                    // physical device properties
                    vkGetPhysicalDeviceProperties(device, &devProps);
                    physicalDevicesProperties_.push_back(devProps);

                    // physical device features
                    vkGetPhysicalDeviceFeatures(device, &devFeatures);
                    physicalDevicesFeatures_.push_back(devFeatures);

                    // physical device queue families properies
                    uint32_t queueFamilyCount{};

                    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
                    queueFamilyProperties_.resize(size_t(queueFamilyCount));
                    vkGetPhysicalDeviceQueueFamilyProperties(
                      device, &queueFamilyCount, queueFamilyProperties_.data());
                }
            }

            void createDevice() {
                queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                // queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
                queueCreateInfo.queueCount = 1;

                deviceCreateInfo_.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
                deviceCreateInfo_.pQueueCreateInfos = &queueCreateInfo;
                deviceCreateInfo_.queueCreateInfoCount = 1;
                deviceCreateInfo_.pEnabledFeatures = &physicalDevicesFeatures_[0];

                // deviceCreateInfo_.enabledExtensionCount
                //   = static_cast<uint32_t>(extensionsNames_.size());
                // deviceCreateInfo_.ppEnabledExtensionNames = extensionsNames_.data();

                if (enableValidationLayers_) {
                    deviceCreateInfo_.enabledLayerCount
                      = static_cast<uint32_t>(validationLayersNames_.size());
                    deviceCreateInfo_.ppEnabledLayerNames = validationLayersNames_.data();
                } else {
                    deviceCreateInfo_.enabledLayerCount = 0;
                }

                if (vkCreateDevice(physicalDevices_[0], &deviceCreateInfo_, nullptr, &device_)
                    != VK_SUCCESS) {
                    throw std::runtime_error("failed to create logical device!");
                }
            }

            bool isDeviceSuitable(size_t id) {
                return physicalDevicesProperties_[id].deviceType
                         == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
                       && physicalDevicesFeatures_[id].geometryShader;
            }

            void displayPhysicalDeviceProperties(size_t id);
            void displayPhysicalDeviceFeatures(size_t id);
            void displayPhysicalDeviceFamiliesProperties(size_t id);

        private:
            std::string applicationName_;
            std::string engineName_;

            bool enableValidationLayers_{};

            // all vk structures must be zero initialized
            VkInstance instance_{};
            VkDevice device_{};

            VkApplicationInfo appInfo_{};
            VkInstanceCreateInfo instanceCreateInfo_{};
            VkDebugUtilsMessengerEXT debugMessenger_{};
            VkDebugUtilsMessengerCreateInfoEXT dbgCreateInfo_{};

            VkDeviceQueueCreateInfo queueCreateInfo{};

            VkDeviceCreateInfo deviceCreateInfo_{};

            // extensions and layers for instance creation
            std::vector<VkExtensionProperties> extensionProperties_;
            std::vector<VkLayerProperties> layerProperties_;
            std::vector<char*> validationLayersNames_;
            std::vector<char*> extensionsNames_;

            // physical devices, properties, features and families
            std::vector<VkPhysicalDevice> physicalDevices_;
            std::vector<VkPhysicalDeviceProperties> physicalDevicesProperties_;
            std::vector<VkPhysicalDeviceFeatures> physicalDevicesFeatures_;
            std::vector<VkQueueFamilyProperties> queueFamilyProperties_;
    };
}
}  // namespace tire
