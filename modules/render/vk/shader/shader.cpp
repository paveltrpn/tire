
#include <fstream>
#include <vector>
#include <vulkan/vk_enum_string_helper.h>

#include "shader.h"
#define ENABLE_DEBUG_OUTPUT
#include "log/log.h"

namespace tire::vk {

ShaderStorage::ShaderStorage( const VkDevice &device )
    : device_{ device } {
}

ShaderStorage::~ShaderStorage() {
    for ( const auto &module : modules_ ) {
        vkDestroyShaderModule( device_, std::get<1>( module ), nullptr );
    }
}

void ShaderStorage::add( const std::string &path, const std::string &name ) {
    std::ifstream file( path, std::ios::ate | std::ios::binary );
    if ( !file.is_open() ) {
        throw std::runtime_error(
            std::format( "failed to open file {}!", path ) );
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer( fileSize );
    file.seekg( 0 );
    file.read( buffer.data(), fileSize );
    file.close();

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = buffer.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>( buffer.data() );

    VkShaderModule module;
    const auto err =
        vkCreateShaderModule( device_, &createInfo, nullptr, &module );
    if ( err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create shader module {} with code {}!",
                         name, string_VkResult( err ) ) );
    } else {
        log::info( "shader module {} created!", name );
    }

    modules_[name] = module;
}

VkShaderModule ShaderStorage::get( const std::string &name ) {
    VkShaderModule module;
    try {
        module = modules_.at( name );
    } catch ( std::out_of_range &e ) {
        log::warning( "module {} not exist!", name );
        return VK_NULL_HANDLE;
    }
    return module;
}

void ShaderStorage::destroy( const std::string &name ) {
    VkShaderModule module;
    try {
        module = modules_.at( name );
    } catch ( std::out_of_range &e ) {
        log::warning( "module {} not exist!", name );
        return;
    }
    vkDestroyShaderModule( device_, module, nullptr );
    modules_.erase( name );
}

void ShaderStorage::list() {
    for ( const auto &key : modules_ ) {
        log::debug( "available shader module: \"{}\"", std::get<0>( key ) );
    }
}

}  // namespace tire::vk
