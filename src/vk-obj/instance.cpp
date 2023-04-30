
#include <algorithm>
#include <iostream>
#include <ranges>
#include <vulkan/vulkan.h>

#include "fmt/format.h"
#include "instance.hpp"

void vkInstanceHandle_c::initialEnumerate() {
    uint32_t extCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
    extProperties_.resize(extCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extCount, extProperties_.data());

    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    layerProperties_.resize(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layerProperties_.data());
}

vkInstanceHandle_c::vkInstanceHandle_c() {
    initialEnumerate();

    VkApplicationInfo appInfo{ .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                               .pApplicationName = "basic",
                               .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
                               .pEngineName = "null engine",
                               .engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
                               .apiVersion = VK_API_VERSION_1_0 };

    std::vector<char*> layerNames;
    std::for_each(layerProperties_.begin(), layerProperties_.end(), [&layerNames](auto& lp) {
        layerNames.push_back(lp.layerName);
    });

    std::vector<char*> extNames;
    std::ranges::for_each(extProperties_,
                          [&extNames](auto& ep) { extNames.push_back(ep.extensionName); });

    VkInstanceCreateInfo instCreateInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = static_cast<uint32_t>(layerProperties_.size()),
        .ppEnabledLayerNames = layerNames.data(),
        .enabledExtensionCount = static_cast<uint32_t>(extProperties_.size()),
        .ppEnabledExtensionNames = extNames.data(),
    };

    auto res = vkCreateInstance(&instCreateInfo, nullptr, &instance_);

    if (res != VK_SUCCESS) {
        fmt::print("can't create vk instance with code - {}", static_cast<int>(res));
        std::exit(0);
    } else {
        fmt::print("vk instance create success!!!\n");
    }
}

void vkInstanceHandle_c::create() {
}

vkInstanceHandle_c::~vkInstanceHandle_c() {
    vkDestroyInstance(instance_, nullptr);
}
