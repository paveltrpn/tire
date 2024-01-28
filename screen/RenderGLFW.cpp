
module;

#include <string>
#include <iostream>
#include <format>
#include <print>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

export module screen:RenderGLFW;

import :Render;
import :RenderGL;
import :RenderVK;
import :RenderSF;

namespace tire {

// ======================================================================================
// =============== OpenGL with GLFW initialization struct ===============================
// ======================================================================================
struct __glfw_gl_Render : __gl_Render {
        __glfw_gl_Render(GLFWwindow* window = nullptr) : window_{ window } {
            if (!window_) {
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
            glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

            glfwMakeContextCurrent(window_);

            glewExperimental = GL_TRUE;

            if (glewInit() != GLEW_OK) {
                std::print("failed to initialize GLEW\n");
                std::exit(1);
            };

            // Выключаем вертикальную синхронизацию (VSYNC)
            glfwSwapInterval(0);

            vendor_ = (const char*)glGetString(GL_VENDOR);
            renderer_ = (const char*)glGetString(GL_RENDERER);
            glVersion_ = (const char*)glGetString(GL_VERSION);
            glslVersion_ = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
        };

        __glfw_gl_Render(const __glfw_gl_Render& rhs) = delete;
        __glfw_gl_Render(__glfw_gl_Render&& ths) = delete;

        __glfw_gl_Render& operator=(const __glfw_gl_Render& rhs) = delete;
        __glfw_gl_Render& operator=(__glfw_gl_Render&& rhs) = delete;

        ~__glfw_gl_Render() override = default;

        void swapBuffers() override {
            glfwSwapBuffers(window_);
        };

    private:
        GLFWwindow* window_;
};

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

// ======================================================================================
// =============== Vulkan with GLFW initialization struct ===============================
// ======================================================================================
struct __glfw_vk_Render : __vk_Render {
        __glfw_vk_Render() {
            initExtensionProperties();
            initValidationLayers();
            createrInstance();
            // initDebugMessenger();
            // initPhysicalDevices();
        }

        __glfw_vk_Render(const __glfw_vk_Render& rhs) = delete;
        __glfw_vk_Render(__glfw_vk_Render&& ths) = delete;

        __glfw_vk_Render& operator=(const __glfw_vk_Render& rhs) = delete;
        __glfw_vk_Render& operator=(__glfw_vk_Render&& rhs) = delete;

        ~__glfw_vk_Render() override {
            vkDestroyInstance(instance_, nullptr);
        }

        void swapBuffers() override{};

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
};

export struct __glfw_sf_Render : __sf_Render {
        __glfw_sf_Render() = default;
        __glfw_sf_Render(const __glfw_sf_Render& rhs) = delete;
        __glfw_sf_Render(__glfw_sf_Render&& ths) = delete;

        __glfw_sf_Render& operator=(const __glfw_sf_Render& rhs) = delete;
        __glfw_sf_Render& operator=(__glfw_sf_Render&& rhs) = delete;

        ~__glfw_sf_Render() override = default;
};

}  // namespace tire
