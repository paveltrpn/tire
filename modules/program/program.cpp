module;

#include <print>
#include <memory>
#include <unordered_map>
#include <span>
#include <vulkan/vulkan.h>
#include <format>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include <glslang/Include/glslang_c_interface.h>
#include <glslang/Public/resource_limits_c.h>

#include <shaderc/shaderc.hpp>

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
            beginCompile();

            for ( auto&& shader : textSrc->sources() ) {
                auto [stage, text] = shader;

                glslang_stage_t s{};

                if ( stage == ShaderStageType::VERTEX ) {
                    s = GLSLANG_STAGE_VERTEX;
                } else if ( stage == ShaderStageType::FRAGMENT ) {
                    s = GLSLANG_STAGE_FRAGMENT;
                }

                const auto bytecode = compile( s, text );
                push( stage, bytecode );
            }

            endCompile();
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

    auto push( ShaderStageType stage, const std::vector<uint32_t> bytecode ) -> void {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = bytecode.size();
        createInfo.pCode = bytecode.data();

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

    auto compile( glslang_stage_t stage, const std::string& text ) -> std::vector<uint32_t> {
        std::vector<uint32_t> result{};

        // 1. Setup the input parameters configuration
        const glslang_input_t input = {
            .language = GLSLANG_SOURCE_GLSL,
            .stage = stage,
            .client = GLSLANG_CLIENT_VULKAN,
            .client_version = GLSLANG_TARGET_VULKAN_1_3,  // Target Vulkan 1.3
            .target_language = GLSLANG_TARGET_SPV,
            .target_language_version = GLSLANG_TARGET_SPV_1_6,  // Target SPIR-V 1.6
            .code = text.c_str(),
            .default_version = 450,
            .default_profile = GLSLANG_CORE_PROFILE,
            .force_default_version_and_profile = false,
            .forward_compatible = false,
            .messages = GLSLANG_MSG_DEFAULT_BIT,
            .resource = glslang_default_resource()  // Uses built-in limits configuration
        };

        // 2. Create and compile the shader object
        glslang_shader_t* shader = glslang_shader_create( &input );

        if ( !glslang_shader_preprocess( shader, &input ) ) {
            std::cerr << "GLSL Preprocess Failed for " << "\n"
                      << glslang_shader_get_info_log( shader ) << "\n"
                      << glslang_shader_get_info_debug_log( shader ) << std::endl;
            glslang_shader_delete( shader );
            return result;
        }

        if ( !glslang_shader_parse( shader, &input ) ) {
            std::cerr << "GLSL Parsing Failed for " << "\n"
                      << glslang_shader_get_info_log( shader ) << "\n"
                      << glslang_shader_get_info_debug_log( shader ) << std::endl;
            glslang_shader_delete( shader );
            return result;
        }

        // 3. Link the shader into a program object
        glslang_program_t* program = glslang_program_create();
        glslang_program_add_shader( program, shader );

        // Use standard Spv and Vulkan structural validation rules during linking
        int msg_mask = GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT;
        if ( !glslang_program_link( program, msg_mask ) ) {
            std::cerr << "GLSL Linking Failed for " << "\n"
                      << glslang_program_get_info_log( program ) << "\n"
                      << glslang_program_get_info_debug_log( program ) << std::endl;
            glslang_program_delete( program );
            glslang_shader_delete( shader );
            return result;
        }

        // 4. Transform the parsed program AST into SPIR-V intermediate binary
        glslang_program_SPIRV_generate( program, input.stage );

        // 5. Extract the raw binary words from the generated program
        size_t size = glslang_program_SPIRV_get_size( program );
        if ( size > 0 ) {
            result.resize( size );
            glslang_program_SPIRV_get( program, result.data() );
        }

        // Capture SPIR-V optimization or generation logs if they exist
        if ( glslang_program_SPIRV_get_messages( program ) ) {
            std::cout << "SPIR-V Messages: " << glslang_program_SPIRV_get_messages( program ) << std::endl;
        }

        // Cleanup resources in reverse order
        glslang_program_delete( program );
        glslang_shader_delete( shader );

        return result;
    }

    auto beginCompile() -> void {
        //
        glslang_initialize_process();
    }

    auto endCompile() -> void {
        //
        glslang_finalize_process();
    }

private:
    std::shared_ptr<ProgramSource> _sources{};
    std::unordered_map<ShaderStageType, VkShaderModule> _modules{};
};

}  // namespace tire

// // Struct to hold our final compiled SPIR-V code
// struct SpirVBinary {
//     std::vector<uint32_t> words;
//     bool success = false;
// };

// SpirVBinary CompileGlslToSpirv(glslang_stage_t stage, const char* shaderSource, const char* fileName) {
//     SpirVBinary result;

//     // 1. Setup the input parameters configuration
//     const glslang_input_t input = {
//         .language = GLSLANG_SOURCE_GLSL,
//         .stage = stage,
//         .client = GLSLANG_CLIENT_VULKAN,
//         .client_version = GLSLANG_TARGET_VULKAN_1_3,       // Target Vulkan 1.3
//         .target_language = GLSLANG_TARGET_SPV,
//         .target_language_version = GLSLANG_TARGET_SPV_1_6, // Target SPIR-V 1.6
//         .code = shaderSource,
//         .default_version = 450,
//         .default_profile = GLSLANG_CORE_PROFILE,
//         .force_default_version_and_profile = 0,
//         .forward_compatible = 0,
//         .messages = GLSLANG_MSG_DEFAULT_BIT,
//         .resource = glslang_default_resource()             // Uses built-in limits configuration
//     };

//     // 2. Create and compile the shader object
//     glslang_shader_t* shader = glslang_shader_create(&input);

//     if (!glslang_shader_preprocess(shader, &input)) {
//         std::cerr << "GLSL Preprocess Failed for " << fileName << "\n"
//                   << glslang_shader_get_info_log(shader) << "\n"
//                   << glslang_shader_get_info_debug_log(shader) << std::endl;
//         glslang_shader_delete(shader);
//         return result;
//     }

//     if (!glslang_shader_parse(shader, &input)) {
//         std::cerr << "GLSL Parsing Failed for " << fileName << "\n"
//                   << glslang_shader_get_info_log(shader) << "\n"
//                   << glslang_shader_get_info_debug_log(shader) << std::endl;
//         glslang_shader_delete(shader);
//         return result;
//     }

//     // 3. Link the shader into a program object
//     glslang_program_t* program = glslang_program_create();
//     glslang_program_add_shader(program, shader);

//     // Use standard Spv and Vulkan structural validation rules during linking
//     int msg_mask = GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT;
//     if (!glslang_program_link(program, msg_mask)) {
//         std::cerr << "GLSL Linking Failed for " << fileName << "\n"
//                   << glslang_program_get_info_log(program) << "\n"
//                   << glslang_program_get_info_debug_log(program) << std::endl;
//         glslang_program_delete(program);
//         glslang_shader_delete(shader);
//         return result;
//     }

//     // 4. Transform the parsed program AST into SPIR-V intermediate binary
//     glslang_program_SPIRV_generate(program, input.stage);

//     // 5. Extract the raw binary words from the generated program
//     size_t size = glslang_program_SPIRV_get_size(program);
//     if (size > 0) {
//         result.words.resize(size);
//         glslang_program_SPIRV_get_ptr(program, result.words.data());
//         result.success = true;
//     }

//     // Capture SPIR-V optimization or generation logs if they exist
//     if (glslang_program_SPIRV_get_messages(program)) {
//         std::cout << "SPIR-V Messages: " << glslang_program_SPIRV_get_messages(program) << std::endl;
//     }

//     // Cleanup resources in reverse order
//     glslang_program_delete(program);
//     glslang_shader_delete(shader);

//     return result;
// }

// int main() {
//     // A sample compute shader string
//     const char* computeShaderGLSL = R"(
//         #version 450
//         layout(local_size_x = 64) in;
//         layout(binding = 0) buffer StorageBuffer {
//             float data[];
//         };
//         void main() {
//             uint idx = gl_GlobalInvocationID.x;
//             data[idx] *= 2.0;
//         }
//     )";

//     // Mandatory: Initialize global glslang states once per program lifespan
//     glslang_initialize_process();

//     std::cout << "Compiling Compute Shader..." << std::endl;
//     SpirVBinary compiledShader = CompileGlslToSpirv(GLSLANG_STAGE_COMPUTE, computeShaderGLSL, "compute.comp");

//     if (compiledShader.success) {
//         std::cout << "Success! Compiled SPIR-V Size: " << compiledShader.words.size() * sizeof(uint32_t) << " bytes." << std::endl;
//     } else {
//         std::cerr << "Compilation failed." << std::endl;
//     }

//     // Mandatory: Cleanup global glslang process states before exit
//     glslang_finalize_process();
//     return 0;
// }