#pragma once

#include <map>
#include <vulkan/vulkan.h>

namespace tire::vk {

struct ShaderStorage final {
    ShaderStorage( const VkDevice &device );

    ShaderStorage( const ShaderStorage &other ) = delete;
    ShaderStorage( ShaderStorage &&other ) = delete;
    ShaderStorage &operator=( const ShaderStorage &other ) = delete;
    ShaderStorage &operator=( ShaderStorage &&other ) = delete;

    ~ShaderStorage();

    void add( const std::string &path, const std::string &name );
    VkShaderModule get( const std::string &name );
    void destroy( const std::string &name );
    void list();

private:
    // non owning
    const VkDevice &device_;
    std::map<std::string, VkShaderModule> modules_{};
};

}  // namespace tire::vk
