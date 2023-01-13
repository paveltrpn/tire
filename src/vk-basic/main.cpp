
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>

#include "fmt/core.h"
#include "fmt/format.h"
#include "GLFW/glfw3.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

static GLFWwindow* wnd;

VkInstance appInstance{};
VkApplicationInfo appInfo{
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = "basic",
    .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
    .pEngineName = "null engine",
    .engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
    .apiVersion = VK_API_VERSION_1_0
};

VkInstanceCreateInfo instCreateInfo{
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pApplicationInfo = &appInfo,
};

std::vector<VkExtensionProperties> extProperties;
std::vector<VkPhysicalDevice> devices;
VkDebugUtilsMessengerEXT debugMessenger;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
                VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void doVulkanStuff() {
    // get extensions
    uint32_t extCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
    extProperties.resize(extCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extCount, extProperties.data());

    for (auto& ep : extProperties) {
        fmt::print("{}\n", ep.extensionName);
    }
    
    // extract extension names from array of VkExtensionProperties to array of c strings
    std::vector<char*> extNames;
    std::for_each(extProperties.begin(), extProperties.end(), 
            [&extNames](auto& ep) {
                extNames.push_back(ep.extensionName);
            });

     // validation layers init
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> layerProperties(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.data());

    for (const auto& l : layerProperties) {
        fmt::print("validation layer - {}\n", l.layerName);
    }

    // extract validation layers names from  array of VkLayerProperties to array of c strings
    std::vector<char*> layerNames;
    std::for_each(layerProperties.begin(), layerProperties.end(), 
            [&layerNames](auto& lp) {
                layerNames.push_back(lp.layerName);
            });

    // instance creation
    instCreateInfo.enabledExtensionCount = extCount;
    instCreateInfo.ppEnabledExtensionNames = extNames.data();
    instCreateInfo.enabledLayerCount = layerCount;
    instCreateInfo.ppEnabledLayerNames = layerNames.data();

    auto res = vkCreateInstance(&instCreateInfo, nullptr, &appInstance);

    if (res != VK_SUCCESS) {
        fmt::print("can't create vk instance with code - {}", static_cast<int>(res));
        std::exit(0);
    } else {
        fmt::print("vk instance create success!!!\n");
    }
   
    VkDebugUtilsMessengerCreateInfoEXT dbgCreateInfo{};
    dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    dbgCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    dbgCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
                                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
                                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    dbgCreateInfo.pfnUserCallback = debugCallback;
    dbgCreateInfo.pUserData = nullptr; // Optional
        
    if (vkCreateDebugUtilsMessengerEXT(appInstance, &dbgCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        fmt::print("failed to set up debug messenger!\n");
        std::exit(1);
    }

    uint32_t devCount = 0;

    vkEnumeratePhysicalDevices(appInstance, &devCount, nullptr);
    if (devCount == 0) {
        fmt::print("no vk physical devices in system\n");
        std::exit(1);
    } else {
        fmt::print("vk physical device count - {}\n", devCount);
    }
    
    devices.resize(size_t(devCount));

    vkEnumeratePhysicalDevices(appInstance, &devCount, devices.data());

    for (size_t i = 0; auto& d : devices) {
        VkPhysicalDeviceProperties devProps;
        vkGetPhysicalDeviceProperties(d, &devProps);
        fmt::print("avaible device {} - {}\n", i++, devProps.deviceName);
    }
}

void clearVulkanStuff() {
    vkDestroyInstance(appInstance, nullptr);
} 

int main() {
	if (glfwInit() != GLFW_TRUE) {
		std::cout << fmt::format("initWindow(): glfwInit() return - GLFW_FALSE!\n");
		std::exit(1);
	}
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    wnd = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);
    
    auto keyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(wnd, 1);
        }
    };

	glfwSetKeyCallback(wnd, keyCallback);
    
    doVulkanStuff();

    while (!glfwWindowShouldClose(wnd)) {
        glfwPollEvents();
    }

    clearVulkanStuff();

    glfwDestroyWindow(wnd);
    glfwTerminate();

    return 0;
}
