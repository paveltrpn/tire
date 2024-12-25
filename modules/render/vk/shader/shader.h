#pragma once

#include <map>
#include <vulkan/vulkan.h>

namespace tire::vk {

struct Shader final {
    Shader( const VkDevice device );

    Shader( const Shader &other ) = delete;
    Shader( Shader &&other ) = delete;
    Shader &operator=( const Shader &other ) = delete;
    Shader &operator=( Shader &&other ) = delete;

    ~Shader();

    void add( const std::string &path, const std::string &name );

private:
    // non owning
    const VkDevice device_;
    std::map<std::string, VkShaderModule> modules_{};
};

}  // namespace tire::vk
