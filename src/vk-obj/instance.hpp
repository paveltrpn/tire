
#pragma once 

#include <vector>
#include <vulkan/vulkan.h>

class vkInstanceHandle_c {
    VkInstance instance_;
    std::vector<VkExtensionProperties> extProperties_;
    std::vector<VkLayerProperties> layerProperties_;

    void initialEnumerate();

    public:
        vkInstanceHandle_c();

        void create();

        ~vkInstanceHandle_c();
};
