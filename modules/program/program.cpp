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
        if ( const auto bytecodeSrc = dynamic_cast<BytecodeProgramSource*>( _sources.get() ) ) {
            for ( auto&& shader : bytecodeSrc->sources() ) {
                auto [stage, bytecode] = shader;
                push( stage, bytecode );
            }
        } else if ( const auto textSrc = dynamic_cast<TextProgramSource*>( _sources.get() ) ) {
            for ( auto&& shader : textSrc->sources() ) {
                auto [stage, text] = shader;
                const auto bytecode = compile( text );
                push( stage, bytecode );
            }
        } else {
            log::fatal()( "Unknown program source type!" );
        }
    };

    [[nodiscard]] auto get( ShaderStageType stage ) const -> VkShaderModule const {
        try {
            return _modules.at( stage );
        } catch ( std::out_of_range& e ) {
            return VK_NULL_HANDLE;
        }
    }

    template <ShaderStageType stage>
    requires ShaderStage<stage> [[nodiscard]] auto get() const -> VkShaderModule {
        try {
            return _modules.at( stage );
        } catch ( std::out_of_range& e ) {
            return VK_NULL_HANDLE;
        }
    }

    auto destroy( ShaderStageType stage ) -> void {
        try {
            auto module = _modules.at( stage );
            vkDestroyShaderModule( Context::instance().device(), module, nullptr );
            _modules.erase( stage );
        } catch ( std::out_of_range& e ) {
            log::warning()( "Unable to destroy! Module \"{}\" not exist!", StagesSuffixMap.at( stage ) );
            return;
        }
    }

private:
    // Create vulkan shader module.
    auto push( ShaderStageType stage, const std::span<uint8_t> bytecode ) -> void {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = bytecode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>( bytecode.data() );

        VkShaderModule module{};
        if ( const auto err = vkCreateShaderModule( Context::instance().device(), &createInfo, nullptr, &module );
             err != VK_SUCCESS ) {
            throw std::runtime_error( std::format( "Failed to create shader module \"{}\" with code {}!",
                                                   StagesSuffixMap.at( stage ), string_VkResult( err ) ) );
        } else {
            log::debug()( "Shader module \"{}\" created!", StagesSuffixMap.at( stage ) );
        }

        _modules[stage] = module;
    }

    auto compile( const std::string& text ) -> std::span<uint8_t> {
        // TODO
        log::fatal()( "TODO" );
        return {};
    }

private:
    std::shared_ptr<ProgramSource> _sources{};
    std::unordered_map<ShaderStageType, VkShaderModule> _modules{};
};

}  // namespace tire