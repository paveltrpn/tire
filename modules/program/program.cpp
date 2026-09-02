module;

#include <memory>
#include <unordered_map>
#include <span>
#include <vulkan/vulkan.h>
#include <format>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "context/context.h"
#include "log/log.h"

export module program : program;

import : definitions;
import : programsource;
import : bytecodesource;
import : textsource;

namespace tire {

// Program is a set of vulkan shader modules loaded from spirv bytecode.
// Each program must contain at least two shader stages - VERTEX and FRAGMENT (despite
// of the vulkan specification demands at least one shader stage - VERTEX for graphics
// pipeline or it can be COMPUTE shader for compute pipeline).
export struct Program final {
    Program( const Program& other ) = delete;
    Program( Program&& other ) = delete;
    auto operator=( const Program& other ) -> Program& = delete;
    auto operator=( Program&& other ) -> Program& = delete;

    ~Program() {
        for ( const auto& module : _modules ) {
            vkDestroyShaderModule( Context::instance().device(), std::get<1>( module ), nullptr );
        }
    }

    Program( std::shared_ptr<ProgramSource> sources )
        : _sources{ std::move( sources ) } {
        auto bytecodeSrc = dynamic_cast<BytecodeProgramSource*>( _sources.get() );
        if ( bytecodeSrc ) {
            const auto srcList = bytecodeSrc->sources();

            for ( auto&& shader : srcList ) {
                auto [stage, bytecode] = shader;
                push( stage, bytecode );
            }

            return;
        }

        auto textSrc = dynamic_cast<TextProgramSource*>( _sources.get() );
        if ( textSrc ) {
            const auto srcList = textSrc->sources();

            log::fatal()( "NOT IMPLEMENTED!" );

            return;
        }
    };

    [[nodiscard]] auto get( ShaderStageType stage ) const -> VkShaderModule const {
        try {
            return _modules.at( stage );
        } catch ( std::out_of_range& e ) {
            return VK_NULL_HANDLE;
        }
    }

    // Return shader vulkan module
    template <ShaderStageType stage>
    requires ShaderStage<stage> [[nodiscard]] auto get() const -> VkShaderModule {
        try {
            return _modules.at( stage );
        } catch ( std::out_of_range& e ) {
            return VK_NULL_HANDLE;
        }
    }

    // auto destroy( const std::string& name ) -> void {
    //     VkShaderModule module;
    //     try {
    //         module = _modules.at( name );
    //     } catch ( std::out_of_range& e ) {
    //         log::warning()( "module {} not exist!", name );
    //         return;
    //     }
    //     vkDestroyShaderModule( Context::instance().device(), module, nullptr );
    //     _modules.erase( name );
    // }

private:
    // Create vulkan shader module.
    auto push( ShaderStageType stage, std::span<uint8_t> bytecode ) -> void {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = bytecode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>( bytecode.data() );

        VkShaderModule module{};
        if ( const auto err = vkCreateShaderModule( Context::instance().device(), &createInfo, nullptr, &module );
             err != VK_SUCCESS ) {
            throw std::runtime_error( std::format( "failed to create shader module {} with code {}!",
                                                   StagesSuffixMap.at( stage ), string_VkResult( err ) ) );
        } else {
            log::debug()( "shader module {} created!", StagesSuffixMap.at( stage ) );
        }

        _modules[stage] = module;
    }

private:
    std::shared_ptr<ProgramSource> _sources{};
    std::unordered_map<ShaderStageType, VkShaderModule> _modules{};
};

}  // namespace tire