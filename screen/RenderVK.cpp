
module;

#include <iostream>
#include <format>
#include <print>

#include <algorithm>
#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
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
    std::print("validation layer: {}", pCallbackData->pMessage);
    return VK_FALSE;
}

VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance,
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

export struct __vk_Render : Render {
        __vk_Render() = default;
        __vk_Render(const __vk_Render& rhs) = delete;
        __vk_Render(__vk_Render&& ths) = delete;

        __vk_Render& operator=(const __vk_Render& rhs) = delete;
        __vk_Render& operator=(__vk_Render&& rhs) = delete;

        ~__vk_Render() override = default;
};

export struct __glfw_vk_Render : __vk_Render {
        __glfw_vk_Render() = default;
        __glfw_vk_Render(const __glfw_vk_Render& rhs) = delete;
        __glfw_vk_Render(__glfw_vk_Render&& ths) = delete;

        __glfw_vk_Render& operator=(const __glfw_vk_Render& rhs) = delete;
        __glfw_vk_Render& operator=(__glfw_vk_Render&& rhs) = delete;

        ~__glfw_vk_Render() override = default;
};

export struct __sdl_vk_Render : __vk_Render {
        __sdl_vk_Render() = default;
        __sdl_vk_Render(const __sdl_vk_Render& rhs) = delete;
        __sdl_vk_Render(__sdl_vk_Render&& ths) = delete;

        __sdl_vk_Render& operator=(const __sdl_vk_Render& rhs) = delete;
        __sdl_vk_Render& operator=(__sdl_vk_Render&& rhs) = delete;

        ~__sdl_vk_Render() override = default;
};

}  // namespace tire
