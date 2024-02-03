
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
            enableValidationLayers_ = config_.get<bool>("enable_validation_layers");

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

            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            // queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
            queueCreateInfo.queueCount = 1;

            createInstance();
            enumeratePhysicalDevices();
        };

        ~__vk_Render() override {
            vkDestroyInstance(instance_, nullptr);
        };

        void displayRenderInfo() override {
            displayExtensionProperties();
            displayValidationLayerProperties();

            for (size_t i = 0; i < physicalDevices_.size(); ++i) {
                displayPhysicalDeviceProperties(i);
                displayPhysicalDeviceFeatures(i);
                displayPhysicalDeviceFamiliesProperties(i);
            }
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

        void createInstance() {
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

            if (enableValidationLayers_) {
                instanceCreateInfo_.enabledLayerCount = static_cast<uint32_t>(layers.size());
                instanceCreateInfo_.ppEnabledLayerNames = layers.data();
                instanceCreateInfo_.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&dbgCreateInfo_;
            }

            // extensions
            std::vector<std::string> eplist{
                "VK_KHR_surface", "VK_KHR_xlib_surface", "VK_EXT_debug_report", "VK_EXT_debug_utils"
            };

            auto extensions = makeExtensionsList(std::nullopt);

            instanceCreateInfo_.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
            instanceCreateInfo_.ppEnabledExtensionNames = extensions.data();

            // instance creation
            auto res = vkCreateInstance(&instanceCreateInfo_, nullptr, &instance_);

            if (res != VK_SUCCESS) {
                throw std::runtime_error(
                  std::format("can't create vk instance with code: {}\n", static_cast<int>(res)));
            }

            if (enableValidationLayers_) {
                if (vkCreateDebugUtilsMessenger(
                      instance_, &dbgCreateInfo_, nullptr, &debugMessenger_)
                    != VK_SUCCESS) {
                    throw std::runtime_error("failed to set up debug messenger!\n");
                }
            }
        }

        void enumeratePhysicalDevices() {
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

        bool isDeviceSuitable(size_t id) {
            return physicalDevicesProperties_[id].deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
                   && physicalDevicesFeatures_[id].geometryShader;
        }

        void displayPhysicalDeviceProperties(size_t id) {
            std::print("physical device properties:\n"
                       "================\n");

            auto& d = physicalDevicesProperties_[id];
            std::print("\tname: {}\n\tid: {}\n\tvendor id: {}\n\tapi: {}\n",
                       d.deviceName,
                       d.deviceID,
                       d.vendorID,
                       d.apiVersion);
        }

        void displayPhysicalDeviceFeatures(size_t id) {
            std::print("physical device features:\n"
                       "================\n");

            auto& d = physicalDevicesFeatures_[id];
            std::print("\trobustBufferAccess:\t{}\n"
                       "\tfullDrawIndexUint32:\t{}\n"
                       "\timageCubeArray:\t{}\n"
                       "\tindependentBlend:\t{}\n"
                       "\tgeometryShader:\t{}\n"
                       "\ttessellationShader:\t{}\n"
                       "\tsampleRateShading:\t{}\n"
                       "\tdualSrcBlend:\t{}\n"
                       "\tlogicOp:\t{}\n"
                       "\tmultiDrawIndirect:\t{}\n"
                       "\tdrawIndirectFirstInstance:\t{}\n"
                       "\tdepthClamp:\t{}\n"
                       "\tdepthBiasClamp:\t{}\n"
                       "\tfillModeNonSolid:\t{}\n"
                       "\tdepthBounds:\t{}\n"
                       "\twideLines:\t{}\n"
                       "\tlargePoints:\t{}\n"
                       "\talphaToOne:\t{}\n"
                       "\tmultiViewport:\t{}\n"
                       "\tsamplerAnisotropy:\t{}\n"
                       "\ttextureCompressionETC2:\t{}\n"
                       "\ttextureCompressionASTC_LDR:\t{}\n"
                       "\ttextureCompressionBC:\t{}\n"
                       "\tocclusionQueryPrecise:\t{}\n"
                       "\tpipelineStatisticsQuery:\t{}\n"
                       "\tvertexPipelineStoresAndAtomics:\t{}\n"
                       "\tfragmentStoresAndAtomics:\t{}\n"
                       "\tshaderTessellationAndGeometryPointSize:\t{}\n"
                       "\tshaderImageGatherExtended:\t{}\n"
                       "\tshaderStorageImageExtendedFormats:\t{}\n"
                       "\tshaderStorageImageMultisample:\t{}\n"
                       "\tshaderStorageImageReadWithoutFormat:\t{}\n"
                       "\tshaderStorageImageWriteWithoutFormat:\t{}\n"
                       "\tshaderUniformBufferArrayDynamicIndexing:\t{}\n"
                       "\tshaderSampledImageArrayDynamicIndexing:\t{}\n"
                       "\tshaderStorageBufferArrayDynamicIndexing:\t{}\n"
                       "\tshaderStorageImageArrayDynamicIndexing:\t{}\n"
                       "\tshaderClipDistance:\t{}\n"
                       "\tshaderCullDistance:\t{}\n"
                       "\tshaderFloat64:\t{}\n"
                       "\tshaderInt64:\t{}\n"
                       "\tshaderInt16:\t{}\n"
                       "\tshaderResourceResidency:\t{}\n"
                       "\tshaderResourceMinLod:\t{}\n"
                       "\tsparseBinding:\t{}\n"
                       "\tsparseResidencyBuffer:\t{}\n"
                       "\tsparseResidencyImage2D:\t{}\n"
                       "\tsparseResidencyImage3D:\t{}\n"
                       "\tsparseResidency2Samples:\t{}\n"
                       "\tsparseResidency4Samples:\t{}\n"
                       "\tsparseResidency8Samples:\t{}\n"
                       "\tsparseResidency16Samples:\t{}\n"
                       "\tsparseResidencyAliased:\t{}\n"
                       "\tvariableMultisampleRate:\t{}\n"
                       "\tinheritedQueries:\t{}\n",
                       d.robustBufferAccess,
                       d.fullDrawIndexUint32,
                       d.imageCubeArray,
                       d.independentBlend,
                       d.geometryShader,
                       d.tessellationShader,
                       d.sampleRateShading,
                       d.dualSrcBlend,
                       d.logicOp,
                       d.multiDrawIndirect,
                       d.drawIndirectFirstInstance,
                       d.depthClamp,
                       d.depthBiasClamp,
                       d.fillModeNonSolid,
                       d.depthBounds,
                       d.wideLines,
                       d.largePoints,
                       d.alphaToOne,
                       d.multiViewport,
                       d.samplerAnisotropy,
                       d.textureCompressionETC2,
                       d.textureCompressionASTC_LDR,
                       d.textureCompressionBC,
                       d.occlusionQueryPrecise,
                       d.pipelineStatisticsQuery,
                       d.vertexPipelineStoresAndAtomics,
                       d.fragmentStoresAndAtomics,
                       d.shaderTessellationAndGeometryPointSize,
                       d.shaderImageGatherExtended,
                       d.shaderStorageImageExtendedFormats,
                       d.shaderStorageImageMultisample,
                       d.shaderStorageImageReadWithoutFormat,
                       d.shaderStorageImageWriteWithoutFormat,
                       d.shaderUniformBufferArrayDynamicIndexing,
                       d.shaderSampledImageArrayDynamicIndexing,
                       d.shaderStorageBufferArrayDynamicIndexing,
                       d.shaderStorageImageArrayDynamicIndexing,
                       d.shaderClipDistance,
                       d.shaderCullDistance,
                       d.shaderFloat64,
                       d.shaderInt64,
                       d.shaderInt16,
                       d.shaderResourceResidency,
                       d.shaderResourceMinLod,
                       d.sparseBinding,
                       d.sparseResidencyBuffer,
                       d.sparseResidencyImage2D,
                       d.sparseResidencyImage3D,
                       d.sparseResidency2Samples,
                       d.sparseResidency4Samples,
                       d.sparseResidency8Samples,
                       d.sparseResidency16Samples,
                       d.sparseResidencyAliased,
                       d.variableMultisampleRate,
                       d.inheritedQueries);
        }

        void displayPhysicalDeviceFamiliesProperties(size_t id) {
            std::print("physical device families properties:\n"
                       "================\n");

            auto& d = queueFamilyProperties_[id];
            std::print("\tqueueFlags:\t{}\n"
                       "\tqueueCount:\t{}\n"
                       "\ttimestampValidBits:\t{}\n"
                       "\tminImageTransferGranularity: not printed\n",
                       d.queueFlags,
                       d.queueCount,
                       d.timestampValidBits);
        }

        std::string applicationName_;
        std::string engineName_;

        bool enableValidationLayers_{};

        // all vk structures must be zero initialized
        VkInstance instance_{};
        VkApplicationInfo appInfo_{};
        VkInstanceCreateInfo instanceCreateInfo_{};
        VkDebugUtilsMessengerEXT debugMessenger_{};
        VkDebugUtilsMessengerCreateInfoEXT dbgCreateInfo_{};

        VkDeviceQueueCreateInfo queueCreateInfo{};

        // extensions and layers for instance creation
        std::vector<VkExtensionProperties> extensionProperties_;
        std::vector<VkLayerProperties> layerProperties_;

        // physical devices, properties, features and families
        std::vector<VkPhysicalDevice> physicalDevices_;
        std::vector<VkPhysicalDeviceProperties> physicalDevicesProperties_;
        std::vector<VkPhysicalDeviceFeatures> physicalDevicesFeatures_;
        std::vector<VkQueueFamilyProperties> queueFamilyProperties_;
};

// ======================================================================================
// =============== Vulkan with GLFW initialization struct ===============================
// ======================================================================================
export struct __glfw_vk_Render : __vk_Render {
        __glfw_vk_Render(const tire::Config& config) : __vk_Render{ config } {
            if (doublebuffer_) {
                glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
            } else {
                glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
            }
        };

        void preFrame() override {};
        void postFrame() override {};
};

// ======================================================================================
// =============== Vulkan with X11 initialization struct ===============================
// ======================================================================================
struct __x11_vk_Render : __vk_Render {
        __x11_vk_Render(const tire::Config& config) : __vk_Render{ config } {};

        void displayRenderInfo() override {};
        void preFrame() override {};
        void postFrame() override {};
};

}  // namespace tire
