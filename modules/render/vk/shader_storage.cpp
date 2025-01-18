
#include <fstream>
#include <vector>
#include <vulkan/vk_enum_string_helper.h>

#include "shader_storage.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SHADER_STORAGE_VK_CPP{ false };

namespace tire::vk {

ShaderStorage::ShaderStorage( const vk::Device *device )
    : device_{ device } {
}

ShaderStorage::~ShaderStorage() {
    for ( const auto &module : modules_ ) {
        vkDestroyShaderModule( device_->handle(), std::get<1>( module ),
                               nullptr );
    }
}

void ShaderStorage::add( const std::filesystem::path &path ) {
    if ( device_->handle() == VK_NULL_HANDLE ) {
        throw std::runtime_error( std::format(
            "can't use shaders before valid logical device is acquired!" ) );
    }

    std::ifstream file( path, std::ios::ate | std::ios::binary );
    if ( !file.is_open() ) {
        throw std::runtime_error(
            std::format( "failed to open file {}!", path.string() ) );
    }

    const auto name = path.stem().string();

    const size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer( fileSize );
    file.seekg( 0 );
    file.read( buffer.data(), fileSize );
    file.close();

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = buffer.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>( buffer.data() );

    VkShaderModule module;
    if ( const auto err = vkCreateShaderModule( device_->handle(), &createInfo,
                                                nullptr, &module );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create shader module {} with code {}!",
                         name, string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_SHADER_STORAGE_VK_CPP>(
            "vk::ShaderStorage: shader module {} created!", name );
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
    vkDestroyShaderModule( device_->handle(), module, nullptr );
    modules_.erase( name );
}

void ShaderStorage::list() {
    for ( const auto &key : modules_ ) {
        log::debug<DEBUG_OUTPUT_SHADER_STORAGE_VK_CPP>(
            "available shader module: \"{}\"", std::get<0>( key ) );
    }
}

void ShaderStorage::fill( const std::vector<std::filesystem::path> &files ) {
    if ( files.size() < 2 ) {
        throw std::runtime_error( std::format(
            "vk::ShaderStorage: pipeline shader storage must "
            "contains at least vertex and fragment shader stages!" ) );
    }

    for ( const auto &item : files ) {
        add( item );
    }
}

}  // namespace tire::vk
